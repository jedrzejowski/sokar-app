#pragma once

#include <QtCore>
#include <QtWidgets>

#include "_classdef.h"

namespace Sokar {
	class DicomGraphics : public QGraphicsView {
	Q_OBJECT
	public:
		explicit DicomGraphics(QWidget *parent);

	protected:

		void scrollContentsBy(int dx, int dy) override;

		DicomScene *getDicomScene() const {
			return (DicomScene *) scene();
		}

		void resizeEvent(QResizeEvent *event) override;
	};
}