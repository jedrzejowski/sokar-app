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
	static QImage* gdcm2Qt(gdcm::Image const &gimage);
	static QImage* rgb2Qt(gdcm::Image const &gimage, char *buffer);
	static QImage* gray22Qt(gdcm::Image const &gimage, char *buffer);
};
