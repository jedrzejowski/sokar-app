//
// Created by adam on 20.05.2021.
//

#include "CachedVolume.hpp"

using namespace SokarAlg;

CachedVolume::CachedVolume() {
}

CachedVolume::~CachedVolume() {
}

const QSharedPointer<const Volume> &CachedVolume::getVolume() const {
	return volume;
}

void CachedVolume::setVolume(const QSharedPointer<const Volume> &newVolume, bool rebuild) {
	CachedVolume::volume = newVolume;

	if (rebuild) {
		rebuildCache();
	}
}

void CachedVolume::rebuildCache() {
	auto size = volume->getSize();
	cache.resize(size);

	forI32space({0, 0, 0}, size, [this](const auto &position) {
		float value = volume->getValue(position);
		cache(position) = value;
	});
}

glm::i32vec3 CachedVolume::getSize() const {
	return volume->getSize();
}

float CachedVolume::getValue(const glm::i32vec3 &position) const {
	return cache(position);
}

