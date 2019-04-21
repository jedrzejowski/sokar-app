#include <iostream>
#include <algorithm>

#include <QTextDocument>
#include <QGraphicsSceneMouseEvent>

#include "windowint.h"

using namespace Sokar;

Monochrome::WindowInt::WindowInt(DataConverter &dataConverter) : Window(dataConverter) {

	text = new QGraphicsTextItem();
	text->setDefaultTextColor(defaultColor);
	text->setPos(0, 0);
	addToGroup(text);

}

void Monochrome::WindowInt::setCenter(TrueInt newCenter) {
	if (newCenter == center)
		return;

	center = newCenter;
	shouldRegen = true;

	regenText();

	emit centerChanged();
}


void Monochrome::WindowInt::setWidth(TrueInt newWidth) {
	if (newWidth == width)
		return;

	if (newWidth < 0)
		newWidth = 0;

	width = newWidth;
	shouldRegen = true;

	regenText();

	emit widthChanged();
}


bool Monochrome::WindowInt::genLUT() {
	if (!shouldRegen) return false;

	signedMove = signedMove ? maxValue : 0;

	double _x0, _x1;
	double _y0 = 0, _y1 = 1.0;

	if (isInversed())
		std::swap(_y0, _y1);


	_x0 = double(center) - double(width) / 2;
	_x1 = double(center) + double(width) / 2;

	_x0 -= rescaleIntercept;
	_x1 -= rescaleIntercept;

	_x0 /= rescaleSlope;
	_x1 /= rescaleSlope;

	a = (_y1 - _y0) / (_x1 - _x0);
	b = _y1 - a * _x1;

	x0 = _x0;
	x1 = _x1;

	if (hasBackground)
		x0 = std::max(backgroundLvl, x0);

	shouldRegen = false;

	return true;
}

void Monochrome::WindowInt::setRescaleIntercept(double rescaleIntercept) {
	if (WindowInt::rescaleIntercept == rescaleIntercept)
		return;

	WindowInt::rescaleIntercept = rescaleIntercept;
	shouldRegen = true;
}

void Monochrome::WindowInt::setRescaleSlope(double rescaleSlope) {
	if (WindowInt::rescaleSlope == rescaleSlope)
		return;

	WindowInt::rescaleSlope = rescaleSlope;
	shouldRegen = true;
}

void Monochrome::WindowInt::setMaxValue(quint64 length) {
	if (WindowInt::maxValue == length)
		return;

	WindowInt::maxValue = length;
	shouldRegen = true;
}

void Monochrome::WindowInt::setSigned(bool isSigned) {
	if (signedMove > 0 == isSigned)
		return;

	signedMove = isSigned ? 1 : 0;
	shouldRegen = true;
}

void Monochrome::WindowInt::setBackgroundLvl(TrueInt backgroundLvl) {
	if (WindowInt::backgroundLvl == backgroundLvl)
		return;

	hasBackground = true;
	WindowInt::backgroundLvl = backgroundLvl;
	shouldRegen = true;
}

void Monochrome::WindowInt::reposition() {

	setPos(scene()->width() - text->document()->size().width(),
		   scene()->height() - text->document()->size().height());
}

void Monochrome::WindowInt::regenText() {
	QString str;

	str += "<b>C</b> " + QString::number((qlonglong) center);
	str += "<br>";
	str += "<b>W</b> " + QString::number((qlonglong) width);

	text->setHtml(str);

	reposition();
}

void Monochrome::WindowInt::pushDefaultValues(TrueInt center, TrueInt width, QString name) {
	defaultWindows << DefaultWindow{center, width, name};
}

void Monochrome::WindowInt::activateDefWin(const WindowInt::DefaultWindow &win) {
	setWidth(win.width);
	setCenter(win.center);
	emit forceRefreshSignal();
}
