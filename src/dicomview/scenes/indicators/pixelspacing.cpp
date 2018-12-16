#include <QtWidgets>
#include "pixelspacing.h"

using namespace Sokar;

//region LineIndicator

LineIndicator::LineIndicator(DataConverter &dataConverter) :
		SceneIndicator(dataConverter),
		pen(QColor(255, 255, 255)) {

	text = newText();
}

uint LineIndicator::getPxLength() const {
	return pxLength;
}

void LineIndicator::setPxLength(uint pxLength) {
	LineIndicator::pxLength = pxLength;
}

void LineIndicator::setText(QString str) {
	text->setHtml(str);

	update();
}

void LineIndicator::update() {

	for (auto &line : lines) {
		removeFromGroup(line);
		delete line;
	}
	lines.clear();

	uint paraLines = 4;

	lines << newLine(0, 0, pxLength, 0);

	auto step = pxLength / paraLines;
	for (uint i = 0; i < paraLines + 1; i++) {
		qreal width = 4.0 / (i % 2 + 1);
		lines << newLine(i * step, -width, i * step, width);
	}

	auto dy = text->document()->size().height() / 2;
	text->setPos(pxLength, 0);
	for (auto &line : lines) {
		line->moveBy(0, dy);
		addToGroup(line);
	}

	realWidth = pxLength + text->document()->size().width();
}

void LineIndicator::reposition() {}

qreal LineIndicator::getRealWidth() const {
	return realWidth;
}

qreal LineIndicator::getRealHeight() const {
	return text->document()->size().height();
}

//endregion

PixelSpacingIndicator::PixelSpacingIndicator(DataConverter &dataConverter) :
		SceneIndicator(dataConverter),
		xLine(dataConverter),
		yLine(dataConverter) {

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
	updateLines();
}

double PixelSpacingIndicator::getYSpacing() const {
	return ySpacing;
}

void PixelSpacingIndicator::setYSpacing(double ySpacing) {
	PixelSpacingIndicator::ySpacing = ySpacing;
	updateLines();
}

uint PixelSpacingIndicator::getXDim() const {
	return xDim;
}

void PixelSpacingIndicator::setXDim(uint xDim) {
	PixelSpacingIndicator::xDim = xDim;

	updateLines();
}

uint PixelSpacingIndicator::getYDim() const {
	return yDim;
}

void PixelSpacingIndicator::setYDim(uint yDim) {
	PixelSpacingIndicator::yDim = yDim;

	updateLines();
}

void PixelSpacingIndicator::reposition() {

	xLine.setPos(
			(scene()->width() - xLine.getPxLength()) / 2,
			scene()->height() - xLine.getRealHeight());

	yLine.setPos(
			scene()->width() - yLine.getPxLength(),
			(scene()->height() - yLine.getRealHeight()) / 2);
}

void PixelSpacingIndicator::updateLines() {
	if (xSpacing == 0) {
		xLine.hide();
	} else {
		xLine.setPxLength(xDim / 2);
		xLine.setText(QString::number(xSpacing * xDim / 2, 'f', 2) + +" <i>mm</i>");
		xLine.show();
	}

	if (ySpacing == 0) {
		yLine.hide();
	} else {
		yLine.setPxLength(yDim / 2);
		yLine.setText(QString::number(ySpacing * yDim / 2, 'f', 2) + +" <i>mm</i>");
		yLine.show();

		// Obracanie
		auto transform = QTransform();
		auto dx = (qreal) yLine.getPxLength(),
				dy = yLine.getRealHeight();

		transform.translate(dx - dy / 2, 0);
		transform.rotate(90);
		transform.translate(-dx / 2, -dy / 2);
		yLine.setTransform(transform);
	}
}

qreal PixelSpacingIndicator::getBottomSpace() {
	return xLine.getRealHeight();
}

qreal PixelSpacingIndicator::getRightSpace() {
	return yLine.getRealHeight();
}