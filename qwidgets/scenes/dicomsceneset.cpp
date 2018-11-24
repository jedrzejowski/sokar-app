#include "dicomsceneset.h"

#include "monochrome2/monochrome2.h"
#include "unsupported/unsupported.h"


using namespace Sokar;


DicomSceneSet::DicomSceneSet(const gdcm::ImageReader *reader) : imageReader(reader) {

	initScenes();
}

void DicomSceneSet::initScenes() {
	auto &image = imageReader->GetImage();

	auto buffLength = image.GetBufferLength();
	auto area = image.GetDimension(0) * image.GetDimension(1);
	resize((int) buffLength / area);

	imageBuffer.resize(buffLength);
	image.GetBuffer(&imageBuffer[0]);

	auto sceneParams = SceneParams();
	sceneParams.imageReader = imageReader;
	sceneParams.imageBuffer = &imageBuffer;

	for (auto &scene : *this) {

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

DicomSceneSet::~DicomSceneSet() {
	delete imageReader;
}