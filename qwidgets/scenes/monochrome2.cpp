#include "monochrome2.h"

using namespace Sokar;


Monochrome2DicomScene::Monochrome2DicomScene(gdcm::Image *gdcmImage) : DicomScene(gdcmImage) {

	const unsigned int *dimension = gdcmImage->GetDimensions();

	dimX = dimension[0];
	dimY = dimension[1];

	originVectorBuffer.resize(gdcmImage->GetBufferLength());
	originBuffer = &originVectorBuffer[0];
	gdcmImage->GetBuffer(originBuffer);
	targetBuffer = new unsigned char[dimX * dimY * 3];

	reloadPixmap();
}

const QPixmap Monochrome2DicomScene::genQPixmap() {

	uchar *buffer = targetBuffer;

	auto *origin8 = (uchar *) &originVectorBuffer[0];
	auto *origin16 = (ushort *) &originVectorBuffer[0];

	switch (gdcmImage->GetPixelFormat()) {
		case gdcm::PixelFormat::UINT8:

			for (uint i = 0; i < dimX * dimY; i++) {
				*buffer++ = (uchar) *origin8;
				*buffer++ = (uchar) *origin8;
				*buffer++ = (uchar) *origin8;

				origin8++;
			}
			break;

		case gdcm::PixelFormat::UINT16:

			for (uint i = 0; i < dimX * dimY; i++) {

				*buffer++ = (uchar) std::min(255, (31768 + *origin16) / 255);
				*buffer++ = (uchar) std::min(255, (31768 + *origin16) / 255);
				*buffer++ = (uchar) std::min(255, (31768 + *origin16) / 255);
				origin16++;
			}
			break;

		default:
			throw Sokar::ImageTypeNotSupportedException();
	}

	return QPixmap::fromImage(QImage(targetBuffer, dimX, dimY, QImage::Format_RGB888));
}


void Monochrome2DicomScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	QGraphicsScene::mousePressEvent(event);
}

void Monochrome2DicomScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
	QGraphicsScene::mouseMoveEvent(event);
}

void Monochrome2DicomScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	QGraphicsScene::mouseReleaseEvent(event);
}
