//
// Created by adam on 05.05.2021.
//

#include "VolumeEnv.hpp"

using namespace SokarAlg;

VolumeEnv::VolumeEnv(const QSharedPointer<const Volume> &child, float envValue)
		: envSize(1, 1, 1), child(child), envValue(envValue) {
}

glm::i32vec3 VolumeEnv::getSize() const {
	return child->getSize() + envSize * 2;
}

float VolumeEnv::getValue(const glm::i32vec3 &position) const {
	auto pos = position - envSize;
	auto size = child->getSize();

	if (0 <= pos.x && pos.x < size.x &&
		0 <= pos.y && pos.y < size.x &&
		0 <= pos.z && pos.z < size.z) {
		return child->getValue(pos);
	}

	return envValue;
}
