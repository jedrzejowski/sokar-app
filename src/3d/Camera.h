#pragma once

#include "./_def.h"
#include <QVector3D>
#include <QMatrix4x4>

namespace Sokar3D {

	class Camera {
	public:
		[[nodiscard]]
		virtual glm::mat4 viewMatrix() const = 0;

		virtual bool uiEvent(QEvent *event);
	};
}
