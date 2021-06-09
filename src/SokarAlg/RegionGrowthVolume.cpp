//
// Created by adam on 09.06.2021.
//

#include "RegionGrowthVolume.hpp"

using namespace SokarAlg;


static const quint8 NOT_VISITED = 0;
static const quint8 VISITED = 1;
static const quint8 PASSES = 2;

const glm::i32vec3 &RegionGrowthVolume::getStartPoint() const {
	return startPoint;
}

void RegionGrowthVolume::setStartPoint(const glm::i32vec3 &startPoint) {
	RegionGrowthVolume::startPoint = startPoint;
}

float RegionGrowthVolume::getMaskValue() const {
	return maskValue;
}

void RegionGrowthVolume::setMaskValue(float maskValue) {
	RegionGrowthVolume::maskValue = maskValue;
}

const Range<float> &RegionGrowthVolume::getIsoLevel() const {
	return isoLevel;
}

void RegionGrowthVolume::setIsoLevel(const Range<float> &isoLevel) {
	RegionGrowthVolume::isoLevel = isoLevel;
}

float RegionGrowthVolume::getValue(const glm::i32vec3 &position) const {
	return (mask(position) & VISITED) && VolumeDecorator::getValue(position);
}

void RegionGrowthVolume::regrowth() {

	QQueue<glm::i32vec3> queue;
	mask.resize(getSize(), 0);

	queue << startPoint;

	while (!queue.isEmpty()) {

		auto position = queue.dequeue();

		mask(position) = mask(position) & VISITED;

		if (isoLevel.isOn(getValue(position))) {
			mask(position) = mask(position) & PASSES;
		}

		glm::i32vec3 next;

		next = position + glm::i32vec3(1, 0, 0);
		if (!(mask(next) & VISITED)) {
			queue << next;
		}

		next = position + glm::i32vec3(-1, 0, 0);
		if (!(mask(next) & VISITED)) {
			queue << next;
		}

		next = position + glm::i32vec3(0, 1, 0);
		if (!(mask(next) & VISITED)) {
			queue << next;
		}

		next = position + glm::i32vec3(0, -1, 0);
		if (!(mask(next) & VISITED)) {
			queue << next;
		}

		next = position + glm::i32vec3(0, 0, 1);
		if (!(mask(next) & VISITED)) {
			queue << next;
		}

		next = position + glm::i32vec3(0, 0, -1);
		if (!(mask(next) & VISITED)) {
			queue << next;
		}
	}
}