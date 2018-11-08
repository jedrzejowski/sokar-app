#include "monochrome2.h"

using namespace Sokar;


Monochrome2DicomScene::Monochrome2DicomScene(gdcm::File &gdcmFile,
											 gdcm::Image &gdcmImage)
		: DicomScene(gdcmFile, gdcmImage) {

	const unsigned int *dimension = gdcmImage.GetDimensions();

	dimX = dimension[0];
	dimY = dimension[1];

	originVectorBuffer.resize(gdcmImage.GetBufferLength());
	gdcmImage.GetBuffer(&originVectorBuffer[0]);
	targetBuffer = new unsigned char[dimX * dimY * 3];

	reloadPixmap();
	
}

bool Monochrome2DicomScene::genQPixmap() {

	uchar *buffer = targetBuffer;

	auto *origin8 = (uchar *) &originVectorBuffer[0];
	auto *origin16 = (ushort *) &originVectorBuffer[0];


	switch (gdcmImage.GetPixelFormat()) {
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

	delete pixmap;

	pixmap = new QPixmap();
	auto qimage = QImage(targetBuffer, dimX, dimY, QImage::Format_RGB888);
	pixmap->convertFromImage(qimage);
	
	return true;
}

Monochrome2DicomScene::~Monochrome2DicomScene() {
	delete targetBuffer;
}
