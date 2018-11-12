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
	size -= bumpsize;
}

void ImgWindow::decSize() {
	size -= bumpsize;
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

ushort ImgWindow::getLeftEdge() {
	return 0;
}

ushort ImgWindow::getRightEdge() {
	return 0;
}

ushort ImgWindow::getMaxLeft() const {
	return maxLeft;
}

void ImgWindow::setMaxLeft(ushort maxLeft) {
	this->maxLeft = maxLeft;
}

ushort ImgWindow::getMaxRight() const {
	return maxRight;
}

void ImgWindow::setMaxRight(ushort maxRight) {
	this->maxRight = maxRight;
}

