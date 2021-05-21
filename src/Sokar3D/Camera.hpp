#pragma once

#include "./Sokar3D.hpp"
#include <QVector3D>

namespace Sokar3D {

	class Camera {
	public:
		[[nodiscard]]
		virtual glm::mat4 viewMatrix() const = 0;

		virtual glm::vec3 position() const = 0;

		virtual bool uiEvent(QEvent *event);
	};
}
