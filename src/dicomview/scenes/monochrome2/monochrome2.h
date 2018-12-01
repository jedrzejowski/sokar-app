#pragma once

#include <QtCore>
#include <QtWidgets>

#include "../dicomscene.h"

#include "windowing/windowint.h"

namespace Sokar::Monochrome2 {

	class Scene : public Sokar::DicomScene {
	Q_OBJECT

	protected:
		Window *imgWindow;

	public:
		explicit Scene(SceneParams &sceneParams);

		~Scene() override;

	protected:

		void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

		bool generatePixmap() override;

		template<typename T>
		void genQPixmapOfType();

		void readAttributes();

	public:
		void toolBarAdjust(DicomToolBar *toolbar) override;

	};


}