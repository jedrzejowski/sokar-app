#include <iostream>
#include <algorithm>
#include "imgwindow.h"

using namespace Sokar;

ImgWindow::ImgWindow() {

}

ImgWindow::~ImgWindow() {
	delete array;
}

int ImgWindow::getCenter() const {
	return center;
}

void ImgWindow::setCenter(int newCenter) {
	if (newCenter == center)
		return;

	auto oldCenter = this->center;
	center = newCenter;

	emit centerChanged(oldCenter, newCenter);
	emit centerChanged();
}

int ImgWindow::getWidth() const {
	return width;
}

void ImgWindow::setWidth(int newWidth) {
	if (newWidth == width)
		return;

	if (newWidth < 0)
		newWidth = 0;

	auto oldWidth = this->width;
	width = newWidth;

	emit widthChanged(oldWidth, newWidth);
	emit widthChanged();
}

int ImgWindow::getMax() const {
	return max;
}

void ImgWindow::setMax(int max) {
	this->max = max;

	delete[] array;
	array = new uchar[getMax()];
}

void ImgWindow::getLUT(uchar *&array, ushort &length) {
	length = (ushort) getMax();
	array = this->array;
}

void ImgWindow::genLUT() {
	int x0 = getLeftEdge(), x1 = getRightEdge();
	uchar y0 = 0, y1 = 255;

	int x = 0;

	auto a = (float) (y1 - y0) / (x1 - x0);
	auto b = y1 - a * x1;
	auto y = b;

	while (x < getMax()) {

		if (y > y1) break;

		array[x] = (uchar) (y > y0 ? y : y0);

		x++;
		y += a;
	}

	while (x < getMax()) {
		array[x] = y1;
		x++;
	}
}

int ImgWindow::getRightEdge() {
	return center + width / 2;
}

int ImgWindow::getLeftEdge() {
	return center - width / 2;
}

void ImgWindow::mvWidth(int d) {
	setWidth(width + d);
}

void ImgWindow::mvCenter(int d) {
	setCenter(center + d);
}

int ImgWindow::getRescaleIntercept() const {
	return rescaleIntercept;
}

void ImgWindow::setRescaleIntercept(int rescaleIntercept) {
	ImgWindow::rescaleIntercept = rescaleIntercept;
}

int ImgWindow::getRescaleSlope() const {
	return rescaleSlope;
}

void ImgWindow::setRescaleSlope(int rescaleSlope) {
	ImgWindow::rescaleSlope = rescaleSlope;
}

int ImgWindow::getMin() const {
	return min;
}

void ImgWindow::setMin(int min) {
	ImgWindow::min = min;
}


