#include <iostream>
#include <algorithm>
#include "imgwindowint.h"

using namespace Sokar;

ImgWindowINT::ImgWindowINT() {

}

ImgWindowINT::~ImgWindowINT() {

}

__int128 ImgWindowINT::getCenter() const {
	return center;
}

void ImgWindowINT::setCenter(__int128 newCenter) {
	if (newCenter == center)
		return;

	auto oldCenter = this->center;
	center = newCenter;

	emit centerChanged();
}

__int128 ImgWindowINT::getWidth() const {
	return width;
}

void ImgWindowINT::setWidth(__int128 newWidth) {
	if (newWidth == width)
		return;

	if (newWidth < 0)
		newWidth = 0;

	auto oldWidth = this->width;
	width = newWidth;

	emit widthChanged();
}

__int128 ImgWindowINT::getRescaleIntercept() const {
	return rescaleIntercept;
}

void ImgWindowINT::setRescaleIntercept(__int128 rescaleIntercept) {
	ImgWindowINT::rescaleIntercept = rescaleIntercept;
}

__int128 ImgWindowINT::getRescaleSlope() const {
	return rescaleSlope;
}

void ImgWindowINT::setRescaleSlope(__int128 rescaleSlope) {
	ImgWindowINT::rescaleSlope = rescaleSlope;
}


void ImgWindowINT::genLUT() {

	__int128 x0 = center - width / 2, x1 = center + width / 2;

	uchar y0 = 0, y1 = 255;

	int x = 0;

	auto a = (float) (y1 - y0) / (x1 - x0);
	auto b = y1 - a * x1;
	auto y = b;

	while (x < length) {

		if (y > y1) break;

		array[x] = (uchar) (y > y0 ? y : y0);

		x++;
		y += a;
	}

	while (x < length) {
		array[x] = y1;
		x++;
	}
}