//
// Created by adam on 05.05.2021.
//

#include "VolumeEnv.h"

using namespace SokarAlg;

VolumeEnv::VolumeEnv(VirtualVolume *child, const glm::vec3 &envSize, float envValue)
		: envSize(envSize), child(child), envValue(envValue) {

	child->setParent(this);
}

glm::vec3 VolumeEnv::getSize() const {
	return child->getSize() + envSize * 2.f;
}

float VolumeEnv::getValue(const glm::vec3 &position) const {
	auto pos = position - envSize;
	auto size = child->getSize();

	if (
			position.x >= 0.f &&
			position.y >= 0.f &&
			position.z >= 0.f &&
			position.x <= size.x &&
			position.y <= size.y &&
			position.z <= size.z
			) {
		return child->getValue(pos);
	}

	return envValue;
}
