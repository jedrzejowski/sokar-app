#pragma once

#include <QtCore>
#include <QtWidgets>
#include <qwidgets/scenes/params.h>
#include "qwidgets/scenes/scene.h"

namespace Sokar {
	class QDicomGraphics;
}

class Sokar::QDicomGraphics : public QGraphicsView {
Q_OBJECT
public:
	explicit QDicomGraphics(QWidget *parent);

protected:

	void scrollContentsBy(int dx, int dy) override;

	DicomScene *getDicomScene() const {
		return (DicomScene *) scene();
	}
};