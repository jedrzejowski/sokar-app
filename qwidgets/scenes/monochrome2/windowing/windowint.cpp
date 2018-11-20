#include <iostream>
#include <algorithm>

#include <QMenu>

#include "windowint.h"

using namespace Sokar::Monochrome2;

WindowINT::WindowINT() {

}

WindowINT::~WindowINT() {
	delete[]  array;
}

void WindowINT::setCenter(__int128 newCenter) {
	if (newCenter == center)
		return;

	center = newCenter;

	emit centerChanged();
}


void WindowINT::setWidth(__int128 newWidth) {
	if (newWidth == width)
		return;

	if (newWidth < 0)
		newWidth = 0;

	width = newWidth;

	emit widthChanged();
}


void WindowINT::genLUT() {

	__int128 x0 = center - width / 2,
			x1 = center + width / 2;

	x0 -= rescaleIntercept;
	x1 -= rescaleIntercept;

	x0 /= rescaleSlope;
	x1 /= rescaleSlope;

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

__int128 WindowINT::getCenter() const {
	return center;
}

__int128 WindowINT::getWidth() const {
	return width;
}

__int128 WindowINT::getRescaleIntercept() const {
	return rescaleIntercept;
}

void WindowINT::setRescaleIntercept(__int128 rescaleIntercept) {
	WindowINT::rescaleIntercept = rescaleIntercept;
}

__int128 WindowINT::getRescaleSlope() const {
	return rescaleSlope;
}

void WindowINT::setRescaleSlope(__int128 rescaleSlope) {
	WindowINT::rescaleSlope = rescaleSlope;
}

int WindowINT::getLength() const {
	return length;
}

void WindowINT::setLength(int length) {
	if (isSigned()) length *= 2;

	array = new Pixel[length];
	WindowINT::length = length;
}

bool WindowINT::isSigned() const {
	return signedMove > 0;
}

void WindowINT::setSigned(bool isSigned) {
	delete[] array;

	setLength(isSigned ? getLength() * 2 : getLength());
}

void WindowINT::selectWindowingIndicator(QWidget *parent,QPoint position) {

	QStringList items;
	items << tr("Spring") << tr("Summer") << tr("Fall") << tr("Winter");

	bool ok;

	auto  menu = QMenu(parent);

//	auto centers = QString::fromStdString(gdcmStringFilter.ToString(gdcm::TagWindowCenter)).split('\\');
//	auto widths = QString::fromStdString(gdcmStringFilter.ToString(gdcm::TagWindowWidth)).split('\\');

	menu.addAction(new QAction("Action 1", this));
	menu.addAction(new QAction("Action 2", this));
	menu.addAction(new QAction("Action 3", this));

	menu.exec(position);

//	QString item = QInputDialog::getItem(
//			this->parent(),
//			tr("Wybierz okienko"),
//			tr("Okienka domyślne:"),
//			items,
//			0,
//			false,
//			&ok);
}
