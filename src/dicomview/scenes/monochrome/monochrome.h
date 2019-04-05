#pragma once

#include <QtCore>
#include <QtWidgets>

#include "../dicomscene.h"
#include "windowing/_window.h"

namespace Sokar::Monochrome {

	class Scene : public Sokar::DicomScene {
	Q_OBJECT

	protected:
		Window *imgWindow;

		Window *getCurrentWindow();

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

		template<typename IntType>
		void findExtremes(TrueInt &max, TrueInt &min);

		void readAttributes();

	public:
		bool acceptMovieMode(MovieMode *movieMode) override;

		void disableMovieMode() override;

	public:
		void toolBarAdjust() override;
	};
}