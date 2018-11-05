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
