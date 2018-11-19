#pragma once

#include <QtCore>
#include <gdcmImageReader.h>
#include "../scene.h"

namespace Sokar::Unsupported {
	class Scene : public Sokar::DicomScene {
	Q_OBJECT

	private:
		QGraphicsTextItem* text22;

	public:
		explicit Scene(const gdcm::ImageReader &imageReader, SceneParams *sceneParams);

		~Scene() override;

	protected:
		bool genQPixmap() override;

	public:
		void reposItems() override;
	};
}

