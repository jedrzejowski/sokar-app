//
// Created by adam on 18.04.2021.
//

#include "GameCamera.hpp"

using namespace Sokar3D;

GameCamera::GameCamera(const glm::vec3 &pos)
        : m_forward(0.0f, 0.0f, -1.0f),
          m_right(1.0f, 0.0f, 0.0f),
          m_up(0.0f, 1.0f, 0.0f),
          m_pos(pos),
          m_yaw(0.0f),
          m_pitch(0.0f) {

    pitch(0);
    yaw(0);
}

static inline void clamp360(float *v) {

    if (*v > 360.0f)
        *v -= 360.0f;
    if (*v < -360.0f)
        *v += 360.0f;
}

void GameCamera::yaw(float degrees) {

    m_yaw += degrees;
    clamp360(&m_yaw);
    m_yawMatrix = glm::mat4(1);
    m_yawMatrix = glm::rotate(m_yawMatrix, m_yaw, glm::vec3(0, 1, 0));

    glm::mat4 rotMat = m_pitchMatrix * m_yawMatrix;
    m_forward = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f) * rotMat;
    m_right = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f) * rotMat;
}

void GameCamera::pitch(float degrees) {

    m_pitch += degrees;
    clamp360(&m_pitch);
    m_pitchMatrix = glm::mat4(1);
    m_pitchMatrix = glm::rotate(m_pitchMatrix, m_pitch, glm::vec3(1, 0, 0));

    glm::mat4 rotMat = m_pitchMatrix * m_yawMatrix;
    m_forward = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f) * rotMat;
    m_up = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f) * rotMat;
}

void GameCamera::walk(float amount) {

    m_pos[0] += amount * m_forward.x;
    m_pos[2] += amount * m_forward.z;
}

void GameCamera::strafe(float amount) {

    m_pos[0] += amount * m_right.x;
    m_pos[2] += amount * m_right.z;
}

glm::mat4 GameCamera::viewMatrix() const {

    glm::mat4 m = m_pitchMatrix * m_yawMatrix;
    glm::translate(m, -m_pos);
    return m;
}
