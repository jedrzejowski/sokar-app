#pragma once

#include <QtCore>
#include <QtWidgets>

#include "../dicomscene.h"
#include "../redgreenblue/redgreenblue.h"

namespace Sokar::LumBlueRed {

	class Scene : public DicomScene {
	Q_OBJECT
	public:
		explicit Scene(SceneParams &sceneParams);
	protected:
		bool generatePixmap() override;

	public:
		void toolBarAdjust() override;
	};
}