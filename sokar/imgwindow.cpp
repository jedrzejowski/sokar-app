#include <iostream>
#include "imgwindow.h"

using namespace Sokar;

ImgWindow::ImgWindow() {

}

void ImgWindow::incCenter() {
	center += bumpsize;
}

void ImgWindow::decCenter() {
	center -= bumpsize;
}

void ImgWindow::incSize() {
	width -= bumpsize;
}

void ImgWindow::decSize() {
	width -= bumpsize;
}

ushort ImgWindow::getCenter() const {
	return center;
}

void ImgWindow::setCenter(ushort center) {
	this->center = center;
}

ushort ImgWindow::getWidth() const {
	return width;
}

void ImgWindow::setWidth(ushort width) {
	this->width = width;
}


ushort ImgWindow::getMax() const {
	return max;
}

void ImgWindow::setMax(ushort max) {
	this->max = max;
}

void ImgWindow::genLUT(uchar *&array, ushort &length) {
	int x0 = getLeftEdge(), x1 = getRightEdge();
	uchar y0 = 0, y1 = 255;

	length = getMax();
	array = new uchar[getMax()];

	float a = (float) (y1 - y0) / (x1 - x0);
	float b = y1 - a * x1;

	if (x0 > 0)
		for (auto i = 0; i < x0; i++)
			array[i] = y0;

	if (x1 < getMax())
		for (auto i = x1; i < getMax(); i++)
			array[i] = y1;

	for (int x = x0, y = y0; x < x1; x++) {

		if (0 < x && x < length)
			array[x] = y;

		y += m;
	}

	for (int i = 0; i < getMax(); i++)
		std::cout << i << ":" << (int) array[i] << std::endl;
}

int ImgWindow::getRightEdge() {
	return center + width / 2;
}

int ImgWindow::getLeftEdge() {
	return center - width / 2;
}

