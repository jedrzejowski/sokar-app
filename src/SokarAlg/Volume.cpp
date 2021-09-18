//
// Created by adam on 14.03.2021.
//

#include "SokarMacro.hpp"
#include "Volume.hpp"

using namespace SokarAlg;

Volume::Volume() {

    sokarTrace();
}

Volume::~Volume() {

    sokarTrace();
}

glm::i32vec3 Volume::clamp(const glm::i32vec3 &index) const {

    return glm::clamp(index, {0, 0, 0}, getSize() - 1);
}

Volume::Point Volume::getPoint(const glm::i32vec3 &position) const {

    return {position, getValue(position)};
}

float Volume::getValueSafe(const glm::i32vec3 &position) const {

    return getValue(clamp(position));
}

Volume::Point Volume::getPointSafe(const glm::i32vec3 &position) const {

    return {position, getValueSafe(position)};
}

bool Volume::isInVolume(const glm::i32vec3 &position) const {

    auto size = getSize();
    return 0 <= position.x and position.x < size.x and
           0 <= position.y and position.y < size.y and
           0 <= position.z and position.z < size.z;
}

VolumeDecorator::VolumeDecorator() {

    sokarTrace();
}

VolumeDecorator::VolumeDecorator(const QSharedPointer<const Volume> &volume)
        : volume(volume) {

    sokarTrace();
}

VolumeDecorator::~VolumeDecorator() {

    sokarTrace();
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
