//
// Created by adam on 18.04.2021.
//

#pragma once

#include "./Sokar3D.hpp"
#include "./Camera.h"

namespace Sokar3D {
	class CenterCamera : public Camera {
	private:
		glm::vec3 lookAtPos = glm::vec3(2.0f, 2.0f, 2.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		float distance;
		float yawAngle = 0.0f;
		float pitchAngle = 0.0f;
		float speedRatio = 4.0f;
		QPoint lastMousePos;

	public:
		[[nodiscard]]
		glm::mat4 viewMatrix() const override;

		[[nodiscard]]
		glm::vec3 position() const override;

		CenterCamera(const glm::vec3 &centerPos, float distance);

		bool uiEvent(QEvent *event) override;

		void yaw(float degrees);
		void pitch(float degrees);
		void zoom(float zoom);
	};
}



