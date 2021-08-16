#include "graphics.h"

#include "src/SokarScene/DicomScene.hpp"

using namespace Sokar;

DicomGraphics::DicomGraphics(QWidget *parent) : QGraphicsView(parent) {

	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

//Aby nie było scrola
void DicomGraphics::scrollContentsBy(int, int) {}

void DicomGraphics::resizeEvent(QResizeEvent *event) {

	if (getDicomScene() != nullptr) {
		getDicomScene()->setSceneRect(0, 0, width(), height());
		getDicomScene()->reposItems();
	}

	QGraphicsView::resizeEvent(event);
}