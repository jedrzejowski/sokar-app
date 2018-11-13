#include <iostream>
#include "imgwindow.h"

using namespace Sokar;

ImgWindow::ImgWindow() {

}

ImgWindow::~ImgWindow() {
	delete array;
}

ushort ImgWindow::getCenter() const {
	return center;
}

void ImgWindow::setCenter(ushort center) {
	this->center = center;

//	emit centerChanged(center);
}

ushort ImgWindow::getWidth() const {
	return width;
}

void ImgWindow::setWidth(ushort width) {
	this->width = width;

//	emit widthChanged(center);
}

ushort ImgWindow::getMax() const {
	return max;
}

void ImgWindow::setMax(ushort max) {
	this->max = max;

//	delete(array);
	array = new uchar[getMax()];
}

void ImgWindow::getLUT(uchar *&array, ushort &length) {
	length = getMax();
	array = this->array;
}

void ImgWindow::genLUT() {
	int x0 = getLeftEdge(), x1 = getRightEdge();
	uchar y0 = 0, y1 = 255;

	auto a = (float) (y1 - y0) / (x1 - x0);
//	float b = y1 - a * x1;

	if (x0 > 0)
		for (auto i = 0; i < x0; i++)
			array[i] = y0;

	if (x1 < getMax())
		for (auto i = x1; i < getMax(); i++)
			array[i] = y1;

	float y = y0;
	for (int x = x0; x < x1; x++) {

		if (0 < x && x < getMax())
			array[x] = (uchar) y;

		y += a;
	}
}

int ImgWindow::getRightEdge() {
	return center + width / 2;
}

int ImgWindow::getLeftEdge() {
	return center - width / 2;
}

void ImgWindow::mvWidth(short d) {
	setWidth(width + d);
}

void ImgWindow::mvCenter(short d) {
	setCenter(center + d);
}


