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

	if (dicomScene() != nullptr)
		dicomScene()->resize(this->width(), this->height());
}

DicomScene *DicomView::dicomScene() {
	return (DicomScene *) scene();
}

void DicomView::setDicomScene(DicomScene *scene) {
	setScene(scene);
	dicomScene()->resize(this->width(), this->height());
}
