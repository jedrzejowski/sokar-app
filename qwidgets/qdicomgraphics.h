#pragma once

#include <QtCore>
#include <QtWidgets>
#include <sokar/sceneparams.h>
#include "dicomscene.h"

namespace Sokar {
	class QDicomGraphics;
}

class Sokar::QDicomGraphics : public QGraphicsView {
Q_OBJECT
public:
	explicit QDicomGraphics(QWidget *parent);

	SceneParams *sceneParams;

	bool isDragging = false;
	int lastX, lastY;

protected:
	void mousePressEvent(QMouseEvent *event) override;

	void mouseReleaseEvent(QMouseEvent *event) override;

	void mouseMoveEvent(QMouseEvent *event) override;

	void scrollContentsBy(int dx, int dy) override;

	DicomScene *getDicomScene() const {
		return (DicomScene *) scene();
	}
};