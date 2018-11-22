#include <QTextDocument>
#include "pixel.h"


Sokar::PixelSpacingIndicator::PixelSpacingIndicator() : pen(defaultColor) {
	xText = new QGraphicsTextItem();
	xText->hide();
	xText->setDefaultTextColor(defaultColor);
	addToGroup(xText);

	yText = new QGraphicsTextItem();
	yText->hide();
	yText->setDefaultTextColor(defaultColor);
	addToGroup(yText);

	xLine = new QGraphicsLineItem(0, 0, 0, 0);
	xLine->hide();
	xLine->setPen(pen);
	addToGroup(xLine);

	yLine = new QGraphicsLineItem(0, 0, 0, 0);
	yLine->hide();
	yLine->setPen(pen);
	addToGroup(yLine);
}

double Sokar::PixelSpacingIndicator::getXSpacing() const {
	return xSpacing;
}

void Sokar::PixelSpacingIndicator::setXSpacing(double xSpacing) {
	PixelSpacingIndicator::xSpacing = xSpacing;
	updateTextX();
}

double Sokar::PixelSpacingIndicator::getYSpacing() const {
	return ySpacing;
}

void Sokar::PixelSpacingIndicator::setYSpacing(double ySpacing) {
	PixelSpacingIndicator::ySpacing = ySpacing;
	updateTextY();
}

uint Sokar::PixelSpacingIndicator::getXDim() const {
	return xDim;
}

void Sokar::PixelSpacingIndicator::setXDim(uint xDim) {
	PixelSpacingIndicator::xDim = xDim;
	xLine->setLine(0, 0, (qreal) xDim / 2, 0);
	updateTextX();
}

uint Sokar::PixelSpacingIndicator::getYDim() const {
	return yDim;
}

void Sokar::PixelSpacingIndicator::setYDim(uint yDim) {
	PixelSpacingIndicator::yDim = yDim;
	yLine->setLine(0, 0, 0, (qreal) yDim / 2);
	updateTextY();
}

void Sokar::PixelSpacingIndicator::reposition() {
	xText->setPos(
			(scene()->width() - xText->document()->size().width()) / 2,
			scene()->height() - xText->document()->size().height());

	xLine->setPos(
			(scene()->width() - xLine->line().length()) / 2,
			scene()->height() - xText->document()->size().height());

	yText->setPos(
			scene()->width() - yText->document()->size().width(),
			(scene()->height() - yText->document()->size().height()) / 2);

	yLine->setPos(
			scene()->width() - yText->document()->size().height(),
			(scene()->height() - yLine->line().length()) / 2);
}

void Sokar::PixelSpacingIndicator::updateTextX() {
	if (xSpacing == 0) {
		xLine->hide();
	}
	else {
		xText->setHtml(QString::number(xSpacing * xDim / 2, 'f', 2) + +" <i>mm</i>");
		xText->show();
		xLine->show();
	}
}

void Sokar::PixelSpacingIndicator::updateTextY() {
	if (ySpacing == 0) {
		yLine->hide();
	}
	else {
		yText->setHtml(QString::number(ySpacing * yDim / 2, 'f', 2) + +" <i>mm</i>");

		// Obracanie
		transform = QTransform();
		auto dx = yText->document()->size().width() / 2,
				dy = yText->document()->size().height() / 2;

		transform.translate(dx, dy);
		transform.rotate(90);
		transform.translate(-dx, -dx);
		yText->setTransform(transform);

		yText->show();
		yLine->show();
	}
}

