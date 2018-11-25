#pragma once

#include <QtCore>
#include <gdcmImageReader.h>
#include "qwidgets/scenes/dicomscene.h"

namespace Sokar::Unsupported {
	class Scene : public Sokar::DicomScene {
	Q_OBJECT

	private:
		QGraphicsTextItem* text22;

	public:
		explicit Scene(SceneParams &sceneParams);

		~Scene() override;

	protected:
		bool generatePixmap() override;

	public:
		void reposItems() override;
	};
}

