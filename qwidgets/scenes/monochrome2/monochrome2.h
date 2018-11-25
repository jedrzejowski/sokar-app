#pragma once

#include <QtCore>
#include <qgraphicssceneevent.h>

#include "qwidgets/scenes/dicomscene.h"

#include "windowing/windowint.h"

namespace Sokar::Monochrome2 {

	class Scene : public Sokar::DicomScene {
	Q_OBJECT

	protected:

		bool isWindowEditing = false;

		Window *imgWindow;

	public:
		explicit Scene(SceneParams &sceneParams);

		~Scene() override;

	protected:

		void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

		void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

		void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

		void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

		bool generatePixmap() override;

		template<typename T>
		void genQPixmapOfType();

		void readAttributes();

	};


}