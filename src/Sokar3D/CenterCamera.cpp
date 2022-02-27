//
// Created by adam on 18.04.2021.
//

#include "./CenterCamera.hpp"
#include <QMouseEvent>
#include <QApplication>

using namespace Sokar3D;

CenterCamera::CenterCamera(
        const glm::vec3 &centerPos,
        float distance
) : lookAtPos(centerPos),
    distance(distance) {
}

float normalizeAngle(float angle) {

    angle = std::fmod(angle, 360);

    if (angle > 180) {
        angle -= 360;
    }

    return glm::radians(angle);
}

glm::mat4 CenterCamera::viewMatrix() const {

    auto pos = position();

    auto rolledUp = glm::rotate(cameraUp, normalizeAngle(rollAngle), pos);

    return glm::lookAt(pos, lookAtPos, rolledUp);
}

bool CenterCamera::uiEvent(QEvent *event) {

    if (event->type() == QEvent::MouseButtonPress) {
        event->accept();
        auto mouseEvent = (QMouseEvent *) event;
        lastMousePos = mouseEvent->pos();
        return true;
    }


    if (event->type() == QEvent::MouseMove) {

        auto keyboard = QApplication::keyboardModifiers();
        auto mouseEvent = (QMouseEvent *) event;

        if (mouseEvent->buttons().testFlag(Qt::LeftButton)) {

            auto qDelta = mouseEvent->pos() - lastMousePos;
            auto delta = glm::vec2(qDelta.x(), qDelta.y()) / speedRatio;

            if (keyboard.testFlag(Qt::ShiftModifier)) {
                event->accept();

            } else if (keyboard.testFlag(Qt::AltModifier)) {
                event->accept();

                rollAngle += delta.x * -1;

            } else {
                event->accept();

                auto nRoll = normalizeAngle(rollAngle) * -1;
                auto sin = glm::sin(nRoll);
                auto cos = glm::cos(nRoll);

                yaw(delta.x * cos - delta.y * sin);
                pitch(delta.x * sin + delta.y * cos);

                lastMousePos = mouseEvent->pos();

                return true;
            }
        }
    }


    if (event->type() == QEvent::Wheel) {
        event->accept();
        auto wheelEvent = (QWheelEvent *) event;
        zoom(wheelEvent->angleDelta().y() / -10 / speedRatio);
        return true;
    }

    return Camera::uiEvent(event);
}

void CenterCamera::yaw(float degrees) {

    yawAngle += degrees;
}

void CenterCamera::pitch(float degrees) {

    pitchAngle += degrees;
}

void CenterCamera::zoom(float zoom) {

    distance += zoom;
}

glm::vec3 CenterCamera::position() const {

    glm::vec3 cameraFront = glm::vec3(1.0f, 0.0f, 0.0f);

    cameraFront.x = cos(normalizeAngle(yawAngle)) * cos(normalizeAngle(pitchAngle));
    cameraFront.y = sin(normalizeAngle(pitchAngle));
    cameraFront.z = sin(normalizeAngle(yawAngle)) * cos(normalizeAngle(pitchAngle));

    cameraFront = glm::normalize(cameraFront) * distance;

    return lookAtPos + cameraFront;
}
