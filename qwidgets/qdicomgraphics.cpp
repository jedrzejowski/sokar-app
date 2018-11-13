#include <iostream>
#include "qdicomgraphics.h"

using namespace Sokar;

QDicomGraphics::QDicomGraphics(QWidget *parent) : QGraphicsView(parent) {

	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

//Aby nie było scrola
void QDicomGraphics::scrollContentsBy(int, int) {}

void QDicomGraphics::mousePressEvent(QMouseEvent *event) {
	event->accept();

	lastX = event->x();
	lastY = event->y();
	isDragging = true;

	QGraphicsView::mousePressEvent(event);
}

void QDicomGraphics::mouseReleaseEvent(QMouseEvent *event) {
	event->accept();

	isDragging = false;

	QGraphicsView::mouseReleaseEvent(event);
}

void QDicomGraphics::mouseMoveEvent(QMouseEvent *event) {

	if (isDragging) {

		int x = event->x(), y = event->y();

		sceneParams->imgWindow.mvCenter(y - lastY);
		sceneParams->imgWindow.mvWidth(x - lastX);

		lastX = x;
		lastY = y;

		getDicomScene()->reloadPixmap();
	}

	QGraphicsView::mouseMoveEvent(event);
}

