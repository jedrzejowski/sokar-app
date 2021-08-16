#pragma once

#include <QtCore>
#include <QtWidgets>
#include "SokarScene/SceneParams.hpp"

#include "../dicomscene.h"

namespace Sokar::RedGreenBlue {

	class Scene : public Sokar::DicomScene {
	Q_OBJECT

	public:
		explicit Scene(SokarScene::SceneParams &sceneParams);
	protected:
		bool generatePixmap() override;
	public:
		void toolBarAdjust() override;
	};

}