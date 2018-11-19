#pragma once

#include <QtCore>
#include <qgraphicssceneevent.h>

#include "qwidgets/scenes/scene.h"

#include "windowing/windowint.h"

namespace Sokar::Monochrome2 {

	class Scene : public Sokar::DicomScene {
	Q_OBJECT

	protected:
		std::vector<char> originVectorBuffer;
		Pixel *targetBuffer;
		uint dimX, dimY;

		bool isWindowEditing = false;
		WindowINT *imgWindowInt;

	public:
		explicit Scene(const gdcm::ImageReader &imageReader, SceneParams *sceneParams);

		~Scene() override;

	private:

		void selectWindowingIndicator();

	protected:

		void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

		void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

		void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

		void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

		bool genQPixmap() override;

		template<typename T>
		void genQPixmapOfType();

		void readAttributes();

		QString genText33() override;

	};


}