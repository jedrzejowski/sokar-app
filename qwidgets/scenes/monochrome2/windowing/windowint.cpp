#include <iostream>
#include <algorithm>

#include <QMenu>
#include <QTextDocument>

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

	regenText();

	emit centerChanged();
}


void WindowInt::setWidth(__int128 newWidth) {
	if (newWidth == width)
		return;

	if (newWidth < 0)
		newWidth = 0;

	width = newWidth;

	regenText();

	emit widthChanged();
}


void WindowInt::genLUT() {

	signedMove = signedMove ? maxValue : 0;

	x0 = center - width / 2;
	x1 = center + width / 2;

	x0 -= rescaleIntercept;
	x1 -= rescaleIntercept;

	x0 /= rescaleSlope;
	x1 /= rescaleSlope;

	a = (float) (y1 - y0) / (x1 - x0);
	b = y1 - a * x1;
}

void WindowInt::setRescaleIntercept(__int128 rescaleIntercept) {
	WindowInt::rescaleIntercept = rescaleIntercept;
}

void WindowInt::setRescaleSlope(__int128 rescaleSlope) {
	WindowInt::rescaleSlope = rescaleSlope;
}

void WindowInt::setMaxValue(quint64 length) {
	WindowInt::maxValue = length;
}

void WindowInt::setSigned(bool isSigned) {
	signedMove = isSigned ? maxValue : 0;
}

void WindowInt::selectWindowingIndicator(QWidget *parent, QPoint position) {

	QStringList items;
	items << tr("Spring") << tr("Summer") << tr("Fall") << tr("Winter");

	bool ok;

	auto menu = QMenu(parent);

//	auto centers = QString::fromStdString(gdcmStringFilter.ToString(gdcm::TagWindowCenter)).split('\\');
//	auto widths = QString::fromStdString(gdcmStringFilter.ToString(gdcm::TagWindowWidth)).split('\\');
//
//	menu.addAction(new QAction("Action 1", this));
//	menu.addAction(new QAction("Action 2", this));
//	menu.addAction(new QAction("Action 3", this));

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
	selectWindowingIndicator();
	SceneIndicator::mousePressEvent(event);
}

