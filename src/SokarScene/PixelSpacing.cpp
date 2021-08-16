#include <QtWidgets>
#include "sokar/exception.h"
#include "PixelSpacing.hpp"

using namespace SokarScene;

//region LineIndicator

LineIndicator::LineIndicator(SokarDicom::DataConverter &dataConverter) :
		SceneIndicator(dataConverter),
		pen(QColor(255, 255, 255)) {

	text = newText();
}

qreal LineIndicator::getPxLength() const {
	return pxLength;
}

void LineIndicator::setPxLength(qreal pxLength) {
	LineIndicator::pxLength = pxLength;
}

void LineIndicator::setText(QString str) {
	text->setHtml(str);
	text->adjustSize();

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

bool LineIndicator::isAlive() {
	return true;
}

//endregion

PixelSpacing::PixelSpacing(SokarDicom::DataConverter &dataConverter) :
		SceneIndicator(dataConverter),
		xLine(dataConverter),
		yLine(dataConverter) {

	xLine.hide();
	addToGroup(&xLine);

	yLine.hide();
	addToGroup(&yLine);
}

qreal PixelSpacing::getXSpacing() const {
	return xSpacing;
}

void PixelSpacing::setXSpacing(qreal xSpacing) {
    PixelSpacing::xSpacing = xSpacing;
}

qreal PixelSpacing::getYSpacing() const {
	return ySpacing;
}

void PixelSpacing::setYSpacing(qreal ySpacing) {
    PixelSpacing::ySpacing = ySpacing;
}

qreal PixelSpacing::getXDim() const {
	return xDim;
}

void PixelSpacing::setXDim(qreal xDim) {
    PixelSpacing::xDim = xDim;
}

qreal PixelSpacing::getYDim() const {
	return yDim;
}

void PixelSpacing::setYDim(qreal yDim) {
    PixelSpacing::yDim = yDim;
}

void PixelSpacing::reposition() {

	updateLines();

	xLine.setPos(
			(scene()->width() - xLine.getPxLength()) / 2,
			scene()->height() - xLine.getRealHeight());

	yLine.setPos(
			scene()->width() - yLine.getPxLength(),
			(scene()->height() - yLine.getRealHeight()) / 2);
}

void PixelSpacing::updateLines() {
	if (xSpacing == 0) {
		xLine.hide();
	} else {
		auto width = qMin(scene()->width() / 2, xDim);
		xLine.setPxLength(width);
		xLine.setText(QString::number(xSpacing * width * xScale, 'f', 2) + +" <i>mm</i>");
		xLine.show();
	}

	if (ySpacing == 0) {
		yLine.hide();
	} else {
		auto height = qMin(scene()->height() / 2, yDim);
		yLine.setPxLength(height);
		yLine.setText(QString::number(ySpacing * height * yScale, 'f', 2) + +" <i>mm</i>");
		yLine.show();

		// Obracanie
		auto transform = QTransform();
		auto dx = yLine.getPxLength(),
				dy = yLine.getRealHeight();

		transform.translate(dx - dy / 2, 0);
		transform.rotate(90);
		transform.translate(-dx / 2, -dy / 2);
		yLine.setTransform(transform);
	}
}

qreal PixelSpacing::getBottomSpace() {
	return xLine.getRealHeight();
}

qreal PixelSpacing::getRightSpace() {
	return yLine.getRealHeight();
}

bool PixelSpacing::isAlive() {
	return true;
}

void PixelSpacing::setScaleTransform(const QTransform &scaleTransform) {

	auto xPoint = QPointF(1, 0);
	auto yPoint = QPointF(0, 1);

	xPoint = scaleTransform.map(xPoint);
	yPoint = scaleTransform.map(yPoint);

	xScale = 1 / qSqrt(qPow(xPoint.x(), 2) + qPow(xPoint.y(), 2));
	yScale = 1 / qSqrt(qPow(yPoint.x(), 2) + qPow(yPoint.y(), 2));

	updateLines();
}