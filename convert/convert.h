#pragma once

#include <gdcmImage.h>

#include <QImage>
#include <QImageWriter>

#define QImageUPtr std::unique_ptr<QImage>

namespace Sokar {
	class ImgConvert;
}

class Sokar::ImgConvert {
public:
	static QImageUPtr gdcm2Qt(gdcm::Image const &gimage);

private:

	static QImageUPtr rgb2Qt(gdcm::Image const &gimage, char *buffer);
	static QImageUPtr gray22Qt(gdcm::Image const &gimage, char *buffer);
};
