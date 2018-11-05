#include "convert.h"
#include "../sokar/exception.h"

using namespace Sokar;

QImage *ImgConvert::gdcm2Qt(gdcm::Image const &gimage) {

	std::vector<char> vbuffer;
	vbuffer.resize(gimage.GetBufferLength());
	char *buffer = &vbuffer[0];
	gimage.GetBuffer(buffer);

	switch (gimage.GetPhotometricInterpretation()) {
		case gdcm::PhotometricInterpretation::RGB:
			return rgb2Qt(gimage, buffer);

		case gdcm::PhotometricInterpretation::MONOCHROME2:
			return gray22Qt(gimage, buffer);

		default:
			throw Sokar::ImageTypeNotSupportedException();
	}
}

QImage *ImgConvert::rgb2Qt(gdcm::Image const &gimage, char *buffer) {
	const unsigned int *dimension = gimage.GetDimensions();

	uint dimX = dimension[0];
	uint dimY = dimension[1];

	if (gimage.GetPixelFormat() != gdcm::PixelFormat::UINT8)
		throw Sokar::ImageTypeNotSupportedException();

	uchar *ubuffer = (uchar *) buffer;
	// QImage::Format_RGB888	13	The image is stored using a 24-bit RGB format (8-8-8).
	return new QImage(ubuffer, dimX, dimY, 3 * dimX, QImage::Format_RGB888);
}

QImage *ImgConvert::gray22Qt(gdcm::Image const &gimage, char *buffer) {
	const unsigned int *dimension = gimage.GetDimensions();

	unsigned int dimX = dimension[0];
	unsigned int dimY = dimension[1];

	uchar *ubuffer;
	uchar *pubuffer;
	short *buffer16;

	switch (gimage.GetPixelFormat()) {
		case gdcm::PixelFormat::UINT8:
			// We need to copy each individual 8bits into R / G and B:
			ubuffer = new unsigned char[dimX * dimY * 3];
			pubuffer = ubuffer;
			for (unsigned int i = 0; i < dimX * dimY; i++) {
				*pubuffer++ = (uchar) *buffer;
				*pubuffer++ = (uchar) *buffer;
				*pubuffer++ = (uchar) *buffer;

				buffer++;
			}

			return new QImage(ubuffer, dimX, dimY, QImage::Format_RGB888);

		case gdcm::PixelFormat::UINT16:
			// We need to copy each individual 16bits into R / G and B (truncate value)
			buffer16 = (short *) buffer;
			ubuffer = new unsigned char[dimX * dimY * 3];
			pubuffer = ubuffer;
			for (uint i = 0; i < dimX * dimY; i++) {

				*pubuffer++ = (uchar) std::min(255, (31768 + *buffer16) / 255);
				*pubuffer++ = (uchar) std::min(255, (31768 + *buffer16) / 255);
				*pubuffer++ = (uchar) std::min(255, (31768 + *buffer16) / 255);
				buffer16++;
			}

			return new QImage(ubuffer, dimX, dimY, QImage::Format_RGB888);

		default:
			throw Sokar::ImageTypeNotSupportedException();
	}
}