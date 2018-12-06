#include "_base.h"

using namespace Sokar;

SceneIndicator::SceneIndicator() : defaultColor(255, 255, 255) {

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
