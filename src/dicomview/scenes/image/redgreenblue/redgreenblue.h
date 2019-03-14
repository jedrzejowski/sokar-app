#pragma once

#include <QtCore>
#include <QtWidgets>

#include "../dicomimagescene.h"

namespace Sokar::RedGreenBlue {

	class Scene : public Sokar::DicomImageScene {
	Q_OBJECT

	public:
		explicit Scene(SceneParams &sceneParams);
	protected:
		bool generatePixmap() override;
	public:
		void toolBarAdjust() override;
	};

}