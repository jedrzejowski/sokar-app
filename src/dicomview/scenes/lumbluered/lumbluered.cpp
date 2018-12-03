#include "sokar/gdcmSokar.h"
#include "lumbluered.h"

using namespace Sokar::LumBlueRed;

Scene::Scene(SceneParams &sceneParams) :
		DicomScene(sceneParams) {

	reloadPixmap();
}

Sokar::Pixel ybr2Pixel(quint8 y, quint8 b, quint8 r) {
	static double red, green, blue;

	//https://www.dabsoft.ch/dicom/3/C.7.6.3.1.2/
	//https://pl.wikipedia.org/wiki/YCbCr
	//http://www.fourcc.org/fccyvrgb.php
	//https://www.programmingalgorithms.com/algorithm/ycbcr-to-rgb?lang=C%2B%2B

	red = green = blue = (255.0 / 219.0) * (y - 16.0);

	red += 255.0 / 224 * 1.402 * (r - 128);
	green -= 255.0 / 224 * 1.772 * (b - 128) * (0.114 / 0.587);
	green -= 255.0 / 224 * 1.402 * (r - 128) * (0.299 / 0.587);
	blue += 255.0 / 224 * 1.772 * (b - 128);

	red = std::max(0.0, std::min(255.0, red));
	green = std::max(0.0, std::min(255.0, green));
	blue = std::max(0.0, std::min(255.0, blue));

	return Sokar::Pixel(quint8(red), quint8(green), quint8(blue));
}

bool Scene::generatePixmap() {
	if (!pixmap.isNull()) return false;

	gdcm::assertTagPresence(gdcmDataSet, gdcm::TagPlanarConfiguration);

	auto planarConfiguration = (ushort) * (gdcmDataSet.GetDataElement(
			gdcm::TagPlanarConfiguration).GetByteValue()->GetPointer());

	if (gdcmImage.GetPixelFormat() != gdcm::PixelFormat::UINT8)
		throw Sokar::ImageTypeNotSupportedException();

	if (gdcmImage.GetPhotometricInterpretation() != gdcm::PhotometricInterpretation::YBR_FULL)
		throw Sokar::ImageTypeNotSupportedException();

	auto *origin = (quint8 * ) & originBuffer[0];
	auto *buffer = &targetBuffer[0];

	quint8 y, b, r;

	for (quint64 i = 0; i < imgDimX * imgDimY; i++) {
		y = *(origin + 0);
		b = *(origin + 1);
		r = *(origin + 2);

		*buffer = ybr2Pixel(y, b, r);

		buffer++;
		origin += 3;
	}

	auto img = QImage((uchar * ) & targetBuffer[0], imgDimX, imgDimY, sizeof(Pixel) * imgDimX, QImage::Format_RGB888);
	pixmap.convertFromImage(img);

	return true;
}

void Scene::toolBarAdjust(DicomToolBar *toolbar) {
	DicomScene::toolBarAdjust(toolbar);

	toolbar->getActionWindowing()->setDisabled(true);
}
