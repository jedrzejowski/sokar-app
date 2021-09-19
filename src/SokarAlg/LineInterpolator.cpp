//
// Created by adam on 19.09.2021.
//

#include "LineInterpolator.hpp"

using namespace SokarAlg;

void LineInterpolator::dicomVolumeChanged() {

}

const VolumeCPtr &LineInterpolator::getVolume() const {

    return volume;
}

void LineInterpolator::setVolume(const VolumeCPtr &new_volume) {

    volume = new_volume;

    dicomVolumeChanged();
}

float LineInterpolator::getIsoLevel() const {

    return iso_level;
}
void LineInterpolator::setIsoLevel(float isoLevel) {

    iso_level = isoLevel;
}

// ----

HalfLineInterpolatorPtr HalfLineInterpolator::New() {

    return HalfLineInterpolatorPtr(new HalfLineInterpolator);
}

glm::vec3 HalfLineInterpolator::interpolate(const Volume::Point &p1, const Volume::Point &p2) const {

    return glm::vec3(p2.position + p1.position) / 2.f;
}

// ----

LinearLineInterpolatorPtr LinearLineInterpolator::New() {

    return LinearLineInterpolatorPtr(new LinearLineInterpolator);
}

glm::vec3 LinearLineInterpolator::interpolate(const Volume::Point &p1, const Volume::Point &p2) const {

    if (areSame(iso_level, p1.value)) {
        return p1.position;
    }
    if (areSame(iso_level, p2.value)) {
        return p2.position;
    }
    if (areSame(p1.value, p2.value)) {
        return p1.position;
    }

    auto mu = (iso_level - p1.value) / (p2.value - p1.value);

    return glm::vec3(p1.position) + mu * glm::vec3(p2.position - p1.position);
}

// ----

SplineLineInterpolatorPtr SplineLineInterpolator::New() {

    return SplineLineInterpolatorPtr(new SplineLineInterpolator);
}

glm::vec3 SplineLineInterpolator::interpolate(const Volume::Point &p1, const Volume::Point &p2) const {

    return glm::vec3(p2.position + p1.position) / 2.f;
}
