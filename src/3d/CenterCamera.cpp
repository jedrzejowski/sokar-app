//
// Created by adam on 18.04.2021.
//

#include "./CenterCamera.h"
#include <QMouseEvent>

using namespace Sokar3D;

CenterCamera::CenterCamera(
		const glm::vec3 &centerPos,
		float distance
) : lookAtPos(centerPos),
	distance(distance) {
}


glm::mat4 CenterCamera::viewMatrix() const {
	return glm::lookAt(position(), lookAtPos, cameraUp);
}

bool CenterCamera::uiEvent(QEvent *event) {
	if (event->type() == QEvent::MouseButtonPress) {
		event->accept();
		auto mouseEvent = (QMouseEvent *) event;
		lastMousePos = mouseEvent->pos();
		return true;
	}


	if (event->type() == QEvent::MouseMove) {
		auto mouseEvent = (QMouseEvent *) event;

		if (mouseEvent->buttons() & Qt::LeftButton) {
			event->accept();

			auto delta = mouseEvent->pos() - lastMousePos;
			yaw(float(delta.x()) / speedRatio);
			pitch(float(delta.y()) / speedRatio);

			lastMousePos = mouseEvent->pos();

			return true;
		}
	}


	if (event->type() == QEvent::Wheel) {
		event->accept();
		auto wheelEvent = (QWheelEvent *) event;
		zoom(wheelEvent->angleDelta().y() / 10 / speedRatio);
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

//	qDebug() << "pitchAngle=" << pitchAngle << " yawAngle=" << yawAngle << " distance=" << distance;

	cameraFront = glm::rotate(cameraFront, glm::radians(pitchAngle), glm::vec3(0, 0, 1));
	cameraFront = glm::rotate(cameraFront, glm::radians(yawAngle), glm::vec3(0, 1, 0));

	cameraFront = glm::normalize(cameraFront) * distance;

//	qDebug() << "cameraFront = " << glm::to_string(cameraFront).c_str();

	return lookAtPos + cameraFront;
}
