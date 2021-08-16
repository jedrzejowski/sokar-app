#pragma once

#include <QtCore>
#include <QtWidgets>

#include "SokarScene/SceneParams.hpp"

#include "../dicomscene.h"
#include "../redgreenblue/redgreenblue.h"

namespace Sokar::LumBlueRed {

	class Scene : public DicomScene {
	Q_OBJECT
	public:
	    explicit Scene(SokarScene::SceneParams &sceneParams);
	protected:
		bool generatePixmap() override;

	public:
		void toolBarAdjust() override;
	};
}