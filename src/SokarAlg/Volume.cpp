//
// Created by adam on 14.03.2021.
//

#include "Volume.hpp"

using namespace SokarAlg;

glm::i32vec3 Volume::clamp(const glm::i32vec3 &index) const {
	return glm::clamp(index, {0, 0, 0}, getSize() - 1);
}

float Volume::getValueSafe(const glm::i32vec3 &position) const {
	return getValue(clamp(position));
}

VolumeDecorator::VolumeDecorator() {
}

VolumeDecorator::VolumeDecorator(const QSharedPointer<const Volume> &volume) : volume(volume) {
}

VolumeDecorator::~VolumeDecorator() {
}

void VolumeDecorator::setVolume(const QSharedPointer<const Volume> &newVolume) {
	volume = newVolume;
	volumeChanged();
}

void VolumeDecorator::volumeChanged() {
}

glm::i32vec3 VolumeDecorator::getSize() const {
	return volume->getSize();
}

float VolumeDecorator::getValue(const glm::i32vec3 &position) const {
	return volume->getValue(position);
}
