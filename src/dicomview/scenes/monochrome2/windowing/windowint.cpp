#include <iostream>
#include <algorithm>

#include <QTextDocument>
#include <QGraphicsSceneMouseEvent>

#include "windowint.h"

using namespace Sokar::Monochrome2;

WindowInt::WindowInt() {

	text = new QGraphicsTextItem();
	text->setDefaultTextColor(defaultColor);
	text->setPos(0, 0);
	addToGroup(text);

}

void WindowInt::setCenter(__int128 newCenter) {
	if (newCenter == center)
		return;

	center = newCenter;
	shouldRegen = true;

	regenText();

	emit centerChanged();
}


void WindowInt::setWidth(__int128 newWidth) {
	if (newWidth == width)
		return;

	if (newWidth < 0)
		newWidth = 0;

	width = newWidth;
	shouldRegen = true;

	regenText();

	emit widthChanged();
}


bool WindowInt::genLUT() {
	if (!shouldRegen) return false;

	signedMove = signedMove ? maxValue : 0;

	double _x0, _x1;
	quint8 _y0 = 0, _y1 = 255;

	if (isInversed())
		std::swap(_y0, _y1);

	_x0 = center - width / 2;
	_x1 = center + width / 2;

	_x0 -= rescaleIntercept;
	_x1 -= rescaleIntercept;

	_x0 /= rescaleSlope;
	_x1 /= rescaleSlope;

	a = (_y1 - _y0) / (_x1 - _x0);
	b = _y1 - a * _x1;

	x0 = static_cast<__int128_t>(_x0);
	x1 = static_cast<__int128_t>(_x1);
	y0 = _y0;
	y1 = _y1;

	shouldRegen = false;

	return true;
}

void WindowInt::setRescaleIntercept(double rescaleIntercept) {
	if (WindowInt::rescaleIntercept == rescaleIntercept)
		return;

	WindowInt::rescaleIntercept = rescaleIntercept;
	shouldRegen = true;
}

void WindowInt::setRescaleSlope(double rescaleSlope) {
	if (WindowInt::rescaleSlope == rescaleSlope)
		return;

	WindowInt::rescaleSlope = rescaleSlope;
	shouldRegen = true;
}

void WindowInt::setMaxValue(quint64 length) {
	if (WindowInt::maxValue == length)
		return;

	WindowInt::maxValue = length;
	shouldRegen = true;
}

void WindowInt::setSigned(bool isSigned) {
	if (signedMove > 0 == isSigned)
		return;

	signedMove = isSigned ? 1 : 0;
	shouldRegen = true;
}

void WindowInt::setBackgroundLvl(__int128_t backgroundLvl) {
	if (WindowInt::backgroundLvl == backgroundLvl)
		return;

	hasBackground = true;
	WindowInt::backgroundLvl = backgroundLvl;
	shouldRegen = true;
}

void WindowInt::reposition() {

	setPos(scene()->width() - text->document()->size().width(),
		   scene()->height() - text->document()->size().height());
}

void WindowInt::regenText() {
	QString str;

	str += "<b>C</b> " + QString::number((qlonglong) center);
	str += "<br>";
	str += "<b>W</b> " + QString::number((qlonglong) width);

	text->setHtml(str);

	reposition();
}

void WindowInt::mousePressEvent(QGraphicsSceneMouseEvent *event) {
//	selectWindowingIndicator();

	if (event->button() == Qt::MouseButton::RightButton)
		selectWindowingIndicator(event);

	SceneIndicator::mousePressEvent(event);
}

void WindowInt::selectWindowingIndicator(QGraphicsSceneMouseEvent *event) {

	QStringList items;

	items << tr("Spring");
//		  << tr("Summer") << tr("Fall") << tr("Winter");

	bool ok;

//	auto menu = QMenu(scene().gr);

//	auto centers = QString::fromStdString(gdcmStringFilter.ToString(gdcm::TagWindowCenter)).split('\\');
//	auto widths = QString::fromStdString(gdcmStringFilter.ToString(gdcm::TagWindowWidth)).split('\\');
//
//	menu.addAction(new QAction("Action 1", this));
//	menu.addAction(new QAction("Action 2", this));
//	menu.addAction(new QAction("Action 3", this));

//	menu.exec(position);

//	QString item = QInputDialog::getItem(
//			this->parent(),
//			tr("Wybierz okienko"),
//			tr("Okienka domyślne:"),
//			items,
//			0,
//			false,
//			&ok);

}
