#include <QTextDocument>
#include "pixelspacing.h"

using namespace Sokar;

PixelSpacingIndicator::PixelSpacingIndicator() : pen(defaultColor) {
	xText.hide();
	xText.setDefaultTextColor(defaultColor);
	addToGroup(&xText);

	yText.hide();
	yText.setDefaultTextColor(defaultColor);
	addToGroup(&yText);

	xLine.hide();
	addToGroup(&xLine);

	yLine.hide();
	addToGroup(&yLine);
}

double PixelSpacingIndicator::getXSpacing() const {
	return xSpacing;
}

void PixelSpacingIndicator::setXSpacing(double xSpacing) {
	PixelSpacingIndicator::xSpacing = xSpacing;
	updateTextX();
}

double PixelSpacingIndicator::getYSpacing() const {
	return ySpacing;
}

void PixelSpacingIndicator::setYSpacing(double ySpacing) {
	PixelSpacingIndicator::ySpacing = ySpacing;
	updateTextY();
}

uint PixelSpacingIndicator::getXDim() const {
	return xDim;
}

void PixelSpacingIndicator::setXDim(uint xDim) {
	PixelSpacingIndicator::xDim = xDim;

	for(auto& line : xLines)


	xLine->setLine(0, 0, (qreal) xDim / 2, 0);
	updateTextX();
}

uint PixelSpacingIndicator::getYDim() const {
	return yDim;
}

void PixelSpacingIndicator::setYDim(uint yDim) {
	PixelSpacingIndicator::yDim = yDim;
	yLine->setLine(0, 0, 0, (qreal) yDim / 2);
	updateTextY();
}

void PixelSpacingIndicator::reposition() {
	xText.setPos(
			(scene()->width() - xText.document()->size().width()) / 2,
			scene()->height() - xText.document()->size().height());

	xLine.setPos(
			(scene()->width() - xLines[0].line().length()) / 2,
			scene()->height() - xText.document()->size().height());

	yText.setPos(
			scene()->width() - yText.document()->size().width(),
			(scene()->height() - yText.document()->size().height()) / 2);

	yLine.setPos(
			scene()->width() - yText.document()->size().height(),
			(scene()->height() - yLines[0].line().length()) / 2);
}

void PixelSpacingIndicator::updateTextX() {
	if (xSpacing == 0) {
		xLine.hide();
	} else {
		xText.setHtml(QString::number(xSpacing * xDim / 2, 'f', 2) + +" <i>mm</i>");
		xText.show();
		xLine.show();
	}
}

void PixelSpacingIndicator::updateTextY() {
	if (ySpacing == 0) {
		yLine.hide();
	} else {
		yText.setHtml(QString::number(ySpacing * yDim / 2, 'f', 2) + +" <i>mm</i>");

		// Obracanie
		transform = QTransform();
		auto dx = yText.document()->size().width() / 2,
				dy = yText.document()->size().height() / 2;

		transform.translate(dx, dy);
		transform.rotate(90);
		transform.translate(-dx, -dx);
		yText.setTransform(transform);

		yText.show();
		yLine.show();
	}
}

qreal PixelSpacingIndicator::getBottomSpace() {
	return xText.document()->size().height();
}

qreal PixelSpacingIndicator::getRightSpace() {
	return yText.document()->size().height();
}

