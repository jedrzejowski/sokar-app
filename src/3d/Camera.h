#pragma once

#include "./_def.h"
#include <QVector3D>
#include <QMatrix4x4>

namespace Sokar3D {

	class Camera {
	public:
		explicit Camera(const glm::vec3 &pos);

		void yaw(float degrees);
		void pitch(float degrees);
		void walk(float amount);
		void strafe(float amount);

		[[nodiscard]]
		glm::mat4 viewMatrix() const;

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
