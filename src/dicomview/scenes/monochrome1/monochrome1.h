#pragma once

#include <QtCore>
#include <QtWidgets>

#include "../dicomscene.h"
#include "../monochrome2/monochrome2.h"

namespace Sokar::Monochrome1 {

	class Scene : public Sokar::Monochrome2::Scene {
	Q_OBJECT

	public:
		explicit Scene(SceneParams &sceneParams);

	};


}