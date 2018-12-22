#include "_base.h"

using namespace Sokar;

SceneIndicator::SceneIndicator(DataConverter &dataConverter) :
		defaultColor(255, 255, 255),
		defaultPen(defaultColor),
		dataConverter(dataConverter) {
}

qreal SceneIndicator::getTopSpace() { return 0; }

qreal SceneIndicator::getBottomSpace() { return 0; }

qreal SceneIndicator::getRightSpace() { return 0; }

qreal SceneIndicator::getLeftSpace() { return 0; }

qreal SceneIndicator::getOffsetRight() const {
	auto out = offsetRight;
	if (offsetRightParent != nullptr)
		out += offsetRightParent->getOffsetTop() + offsetRightParent->getRightSpace();
	return out;
}

void SceneIndicator::setOffsetRight(qreal offsetRight) {
	SceneIndicator::offsetRight = offsetRight;
}

qreal SceneIndicator::getOffsetTop() const {
	auto out = offsetTop;
	if (offsetTopParent != nullptr)
		out += offsetTopParent->getOffsetTop() + offsetTopParent->getTopSpace();
	return out;
}

void SceneIndicator::setOffsetTop(qreal offsetTop) {
	SceneIndicator::offsetTop = offsetTop;
}

qreal SceneIndicator::getOffsetLeft() const {
	auto out = offsetLeft;
	if (offsetLeftParent != nullptr)
		out += offsetLeftParent->getOffsetTop() + offsetLeftParent->getLeftSpace();
	return out;
}

void SceneIndicator::setOffsetLeft(qreal offsetLeft) {
	SceneIndicator::offsetLeft = offsetLeft;
}

qreal SceneIndicator::getOffsetBottom() const {
	auto out = offsetBottom;
	if (offsetBottomParent != nullptr)
		out += offsetBottomParent->getOffsetTop() + offsetBottomParent->getBottomSpace();
	return out;
}

void SceneIndicator::setOffsetBottom(qreal offsetBottom) {
	SceneIndicator::offsetBottom = offsetBottom;
}

SceneIndicator *SceneIndicator::getOffsetRightParent() const {
	return offsetRightParent;
}

void SceneIndicator::setOffsetRightParent(SceneIndicator *offsetRightParent) {
	SceneIndicator::offsetRightParent = offsetRightParent;
}

SceneIndicator *SceneIndicator::getOffsetTopParent() const {
	return offsetTopParent;
}

void SceneIndicator::setOffsetTopParent(SceneIndicator *offsetTopParent) {
	SceneIndicator::offsetTopParent = offsetTopParent;
}

SceneIndicator *SceneIndicator::getOffsetLeftParent() const {
	return offsetLeftParent;
}

void SceneIndicator::setOffsetLeftParent(SceneIndicator *offsetLeftParent) {
	SceneIndicator::offsetLeftParent = offsetLeftParent;
}

SceneIndicator *SceneIndicator::getOffsetBottomParent() const {
	return offsetBottomParent;
}

void SceneIndicator::setOffsetBottomParent(SceneIndicator *offsetBottomParent) {
	SceneIndicator::offsetBottomParent = offsetBottomParent;
}

QGraphicsLineItem *SceneIndicator::newLine(qreal x1, qreal y1, qreal x2, qreal y2) {
	auto line = new QGraphicsLineItem(x1, y1, x2, y2, this);
	line->setPen(defaultPen);
	return line;
}

QGraphicsTextItem *SceneIndicator::newText(const QString &string) {
	auto text = new QGraphicsTextItem(this);
	text->setDefaultTextColor(defaultColor);
	text->setHtml(string);
	text->adjustSize();
	return text;
}

QString SceneIndicator::wrapAsHtml(QStringList lines, bool right) {


	const static QString htmlHead = QObject::tr(
			"<html>"
			"<head>"
			"<style>"
			"body{font-size:medium;background-color:rgba(0,0,0,0.5);}"
			"unit{font-style:italic;font-size:small;}"
			"value{font-weight:500;}"
			"div{white-space: nowrap;}"
			"</style>"
			"</head>"
			"<body>");

	QString align = QString("align='%1'").arg(
			right ? "right" : "left"
	);

	for (auto &line : lines)
		line = QString("<div class='line' %1>%2</div>").arg(
				align,
				line
		);

	return htmlHead + lines.join("") + "</body></html>";
}
