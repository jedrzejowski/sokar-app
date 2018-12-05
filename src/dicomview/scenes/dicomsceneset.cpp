#include "sokar/gdcmSokar.h"

#include "dicomsceneset.h"

#include "unsupported/unsupported.h"
#include "monochrome/monochrome.h"
#include "redgreenblue/redgreenblue.h"
#include "lumbluered/lumbluered.h"

using namespace Sokar;

DicomSceneSet::DicomSceneSet(const gdcm::ImageReader *reader, QObject *parent) :
		QObject(parent),
		imageReader(reader) {

	gdcmStringFilter.SetFile(imageReader->GetFile());

	initScenes();
}

DicomSceneSet::~DicomSceneSet() {
	for (auto &datasetViewer : dataSetViewers)
		delete datasetViewer;

	delete imageReader;
}

void DicomSceneSet::initScenes() {
	auto &image = imageReader->GetImage();
	auto &dataset = imageReader->GetFile().GetDataSet();

	int numberOfFrames = 1;
	if (dataset.FindDataElement(gdcm::TagNumberOfFrames))
		numberOfFrames = QString::fromStdString(gdcmStringFilter.ToString(gdcm::TagNumberOfFrames)).toInt();
	vector.resize(numberOfFrames);

	imageBuffer.resize(image.GetBufferLength());
	image.GetBuffer(&imageBuffer[0]);

	auto sceneParams = SceneParams();
	sceneParams.imgSize = image.GetBufferLength() / numberOfFrames;
	sceneParams.dicomSceneSet = this;
	sceneParams.imageReader = imageReader;
	sceneParams.imageBuffer = &imageBuffer;

	for (auto &scene : vector) {

		try {

			switch (image.GetPhotometricInterpretation()) {
				case gdcm::PhotometricInterpretation::MONOCHROME1:
				case gdcm::PhotometricInterpretation::MONOCHROME2:
					scene = new Sokar::Monochrome::Scene(sceneParams);
					break;

				case gdcm::PhotometricInterpretation::RGB:
					scene = new Sokar::RedGreenBlue::Scene(sceneParams);
					break;

				case gdcm::PhotometricInterpretation::YBR_FULL:
				case gdcm::PhotometricInterpretation::YBR_FULL_422://Nope
				case gdcm::PhotometricInterpretation::YBR_PARTIAL_422://Nope
				case gdcm::PhotometricInterpretation::YBR_PARTIAL_420://Nope
				case gdcm::PhotometricInterpretation::YBR_ICT://Nope
				case gdcm::PhotometricInterpretation::YBR_RCT://Nope
					scene = new Sokar::LumBlueRed::Scene(sceneParams);
					break;

				default:
					throw Sokar::ImageTypeNotSupportedException();
			}

		} catch (Sokar::ImageTypeNotSupportedException &) {
			scene = new Sokar::Unsupported::Scene(sceneParams);
		}

		sceneParams.frame++;
	}
}