#include "dicomview.h"

#include <QtCore>

using namespace Sokar;

DicomView::DicomView(QWidget *parent) : QGraphicsView(parent) {
	setMouseTracking(true);

	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void DicomView::resizeEvent(QResizeEvent *event) {
	QGraphicsView::resizeEvent(event);

	
	if (dicomScene() != nullptr) {
		dicomScene()->setSceneRect(0, 0, this->width(), this->height());
		dicomScene()->reposItems();
	}
}

void DicomView::setDicomScene(DicomScene *scene) {
	
	scene->setSceneRect(0, 0, this->width(), this->height());
	scene->reposItems();
	setScene(scene);
	
}

void DicomView::scrollContentsBy(int dx, int dy) {}
