//
// Created by adam on 18.04.2021.
//

#pragma once

#include "./Sokar3D.hpp"
#include "./Camera.h"

namespace Sokar3D {

	class GameCamera : public Camera {
	public:
		explicit GameCamera(const glm::vec3 &pos);

		void yaw(float degrees);
		void pitch(float degrees);
		void walk(float amount);
		void strafe(float amount);

		[[nodiscard]]
		glm::mat4 viewMatrix() const override;

	private:
		glm::vec3 m_forward;
		glm::vec3 m_right;
		glm::vec3 m_up;
		glm::vec3 m_pos;
		float m_yaw;
		float m_pitch;
		glm::mat4 m_yawMatrix;
		glm::mat4 m_pitchMatrix;
	};
}

