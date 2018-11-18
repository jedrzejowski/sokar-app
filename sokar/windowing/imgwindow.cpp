#include <iostream>
#include <algorithm>
#include "imgwindow.h"

using namespace Sokar;


ImgWindow::ImgWindow() {

}

ImgWindow::~ImgWindow() {
	delete array;
}

ushort ImgWindow::getMaxBit() const {
	return maxBit;
}

void ImgWindow::setMaxBit(ushort maxBit) {
	ImgWindow::maxBit = maxBit;

	if (maxBit > 16) {
		//TODO
	}

	delete[] array;
	length = (1 << maxBit) - 1;
	array = new Pixel[length];
}