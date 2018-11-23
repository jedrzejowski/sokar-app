#include "_base.h"

Sokar::SceneIndicator::SceneIndicator() : defaultColor(255, 255, 255) {

}

void Sokar::SceneIndicator::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	qDebug("DEBIL");
	QGraphicsItem::mousePressEvent(event);
}

void Sokar::SceneIndicator::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
	QGraphicsItem::mouseMoveEvent(event);
}

void Sokar::SceneIndicator::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	QGraphicsItem::mouseReleaseEvent(event);
}

void Sokar::SceneIndicator::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
	QGraphicsItem::mouseDoubleClickEvent(event);
}
