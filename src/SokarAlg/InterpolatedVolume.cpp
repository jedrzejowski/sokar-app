//
// Created by adam on 20.05.2021.
//

#include "InterpolatedVolume.hpp"
#include "VolumeInterpolator.hpp"

using namespace SokarAlg;

//region getters & setters

glm::i32vec3 InterpolatedVolume::getSize() const {
	return size;
}

void InterpolatedVolume::setSize(const glm::i32vec3 &newSize) {
	size = newSize;
}

const InterpolatedVolume::SpaceTranslator &InterpolatedVolume::getSpaceTranslator() const {
	return spaceTranslator;
}

void InterpolatedVolume::setSpaceTranslator(const InterpolatedVolume::SpaceTranslator &newSpaceTranslator) {
	spaceTranslator = newSpaceTranslator;
}

const QSharedPointer<VolumeInterpolator> &InterpolatedVolume::getInterpolator() const {
	return interpolator;
}

void InterpolatedVolume::setInterpolator(const QSharedPointer<VolumeInterpolator> &newInterpolator) {
	interpolator = newInterpolator;
	update();
}

const QSharedPointer<const Volume> &InterpolatedVolume::getVolume() const {
	return volume;
}

void InterpolatedVolume::setVolume(const QSharedPointer<const Volume> &newVolume) {
	volume = newVolume;
	update();
}

//endregion


float InterpolatedVolume::getValue(const glm::i32vec3 &position) const {

	return interpolator->interpolate(spaceTranslator(position));
}

void InterpolatedVolume::update() {
	if (interpolator != nullptr && volume != nullptr) {
		interpolator->setVolume(volume);
	}
}
