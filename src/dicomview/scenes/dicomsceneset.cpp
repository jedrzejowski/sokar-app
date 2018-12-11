#include "sokar/gdcmSokar.h"

#include "dicomsceneset.h"

#include "unsupported/unsupported.h"
#include "monochrome/monochrome.h"
#include "redgreenblue/redgreenblue.h"
#include "lumbluered/lumbluered.h"

using namespace Sokar;

DicomSceneSet::DicomSceneSet(const gdcm::ImageReader *reader, QObject *parent) :
		QObject(parent),
		imageReader(reader),
		gdcmFile(reader->GetFile()),
		gdcmDataSet(gdcmFile.GetDataSet()),
		gdcmImage(reader->GetImage()) {

	gdcmStringFilter.SetFile(imageReader->GetFile());

	initScenes();
}

DicomSceneSet::~DicomSceneSet() {
	for (auto &datasetViewer : dataSetViewers)
		delete datasetViewer;

	delete imageReader;
}

void DicomSceneSet::initScenes() {

	if (gdcmDataSet.FindDataElement(gdcm::TagNumberOfFrames))
		numberOfFrames = QString::fromStdString(gdcmStringFilter.ToString(gdcm::TagNumberOfFrames)).toInt();
	vector.resize(numberOfFrames);

	imageBuffer.resize(gdcmImage.GetBufferLength());
	gdcmImage.GetBuffer(&imageBuffer[0]);

	auto sceneParams = SceneParams();
	sceneParams.imgSize = gdcmImage.GetBufferLength() / numberOfFrames;
	sceneParams.dicomSceneSet = this;
	sceneParams.imageReader = imageReader;
	sceneParams.imageBuffer = &imageBuffer;

	for (auto &scene : vector) {

		try {

			switch (gdcmImage.GetPhotometricInterpretation()) {
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
				case gdcm::PhotometricInterpretation::YBR_RCT://Nopew
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

qreal DicomSceneSet::getFrameTime() {
	if (numberOfFrames == 1)
		return 0;

	static gdcm::Tag
			TagFrameTime(0x0018, 0x1063);

	if (gdcmDataSet.FindDataElement(TagFrameTime)) {
		auto frameTime = QString::fromStdString(gdcmStringFilter.ToString(TagFrameTime)).toDouble();
		return frameTime;
	}

	return 0;
}

const QString &DicomSceneSet::getTitle() {
	if (!title.isEmpty()) return title;

	title.append("To jest <b>tytuł</b> z dicom seta");

	return title;
}