#pragma once

#include <QtCore>
#include <gdcmImageReader.h>

#include "../dicomscene.h"

namespace Sokar {
	class ExceptionScene : public Sokar::DicomScene {
	Q_OBJECT

	private:
		QGraphicsTextItem* text22;

	public:
		explicit Scene(SceneParams &sceneParams);

		~Scene() override;

	protected:
		bool generatePixmap() override;

	public:
		void toolBarAdjust(DicomToolBar *toolbar) override;

	public:
		void reposItems() override;
	};
}

