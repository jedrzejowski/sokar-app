//
// Created by adam on 05.05.2021.
//

#include "VolumeEnv.h"

using namespace SokarAlg;

VolumeEnv::VolumeEnv(float envValue, VirtualVolume *child)
		: envValue(envValue), child(child) {

	child->setParent(this);
}

glm::u32vec3 VolumeEnv::getSize() const {
	return child->getSize() + glm::u32vec3(2);
}

float VolumeEnv::getValue(quint32 x, quint32 y, quint32 z) const {
	if (x == 0 || y == 0 || z == 0) {
		return envValue;
	}

	auto size = child->getSize();

	if (x == size.x + 1 || y == size.y + 1 || z == size.z + 1) {
		return envValue;
	}

	return child->getValue(x - 1, y - 1, z - 1);
}
