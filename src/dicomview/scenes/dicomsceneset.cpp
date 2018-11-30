#include "sokar/gdcmSokar.h"

#include "dicomsceneset.h"

#include "monochrome2/monochrome2.h"
#include "unsupported/unsupported.h"


using namespace Sokar;


DicomSceneSet::DicomSceneSet(const gdcm::ImageReader *reader, QObject *parent) :
		QObject(parent),
		imageReader(reader) {

	initScenes();
}


DicomSceneSet::~DicomSceneSet() {
	for (auto &datasetViewer : dataSetViewers)
		delete datasetViewer;

	delete imageReader;
}


void DicomSceneSet::initScenes() {
	auto &image = imageReader->GetImage();

	auto buffLength = image.GetBufferLength();
	auto imgSize = image.GetDimension(0) * image.GetDimension(1) * gdcm::getPixelByteSize(imageReader->GetFile());

	vector.resize((int) buffLength / imgSize);

	imageBuffer.resize(buffLength);
	image.GetBuffer(&imageBuffer[0]);

	auto sceneParams = SceneParams();
	sceneParams.dicomSceneSet = this;
	sceneParams.imageReader = imageReader;
	sceneParams.imageBuffer = &imageBuffer;

	for (auto &scene : vector) {

		try {

			switch (image.GetPhotometricInterpretation()) {
				case gdcm::PhotometricInterpretation::MONOCHROME2:
					scene = new Sokar::Monochrome2::Scene(sceneParams);
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