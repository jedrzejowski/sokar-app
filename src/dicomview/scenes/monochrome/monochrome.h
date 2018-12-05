#pragma once

#include <QtCore>
#include <QtWidgets>

#include "../dicomscene.h"

#include "windowing/windowint.h"

namespace Sokar::Monochrome {

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

		template<typename IntType>
		void genQPixmapOfType();

		template<typename IntType, typename WinClass>
		void genQPixmapOfTypeWidthWindow();

		template<typename IntType, typename WinClass>
		void genQPixmapOfTypeWidthWindowThread(quint64 from, quint64 to);

		void readAttributes();

	public:
		void toolBarAdjust(DicomToolBar *toolbar) override;

	};


}