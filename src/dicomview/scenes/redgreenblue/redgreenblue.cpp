#include "sokar/gdcmSokar.h"
#include "redgreenblue.h"
#include "../../dicomview.h"

using namespace Sokar;

RedGreenBlue::Scene::Scene(SokarScene::SceneParams &sceneParams) :
		DicomScene(sceneParams) {

	reloadPixmap();
}

bool RedGreenBlue::Scene::generatePixmap() {
	if (!pixmap.isNull()) return false;

	const static gdcm::Tag
			TagPlanarConfiguration(0x0028, 0x0006);

	gdcm::assertTagPresence(gdcmDataSet, TagPlanarConfiguration);

	auto planarConfiguration = dataConverter.toUShort(TagPlanarConfiguration);

	switch (gdcmImage.GetPixelFormat()) {
		case gdcm::PixelFormat::UINT8: {

			auto *origin = (quint8 *) &originBuffer[0];
			auto *buffer = &targetBuffer[0];
			quint64 nextOffset = 0, redOffset = 0, greenOffset = 0, blueOffset = 0;

			if (planarConfiguration == 0) {
				nextOffset = 3;
				redOffset = 0;
				greenOffset = 1;
				blueOffset = 2;
			} else if (planarConfiguration == 1) {
				nextOffset = 1;
				redOffset = 0;
				greenOffset = 1 * imgDimX * imgDimY;
				blueOffset = 2 * imgDimX * imgDimY;
			} else throw Sokar::ImageTypeNotSupportedException();

			for (quint64 i = 0; i < imgDimX * imgDimY; i++) {
				*buffer = Pixel(
						*(origin + redOffset),
						*(origin + greenOffset),
						*(origin + blueOffset)
				);

				buffer++;
				origin += nextOffset;
			}
		}
			break;

		case gdcm::PixelFormat::UINT16:
			qDebug("Szach Mat xD, mam taki plik");
			throw Sokar::ImageTypeNotSupportedException();

		default:
			throw Sokar::ImageTypeNotSupportedException();
	}

	pixmap.convertFromImage(qImage);

	return true;
}

void RedGreenBlue::Scene::toolBarAdjust() {
	SokarScene::DicomScene::toolBarAdjust();

	auto *toolBar = getDicomView()->getToolBar();

	toolBar->getActionWindowing()->setDisabled(true);
}
