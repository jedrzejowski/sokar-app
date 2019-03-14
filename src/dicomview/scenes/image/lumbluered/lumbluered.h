#pragma once

#include <QtCore>
#include <QtWidgets>

#include "../dicomimagescene.h"
#include "../redgreenblue/redgreenblue.h"

namespace Sokar::LumBlueRed {

	class Scene : public DicomImageScene {
	Q_OBJECT
	public:
		explicit Scene(SceneParams &sceneParams);
	protected:
		bool generatePixmap() override;

	public:
		void toolBarAdjust() override;
	};
}