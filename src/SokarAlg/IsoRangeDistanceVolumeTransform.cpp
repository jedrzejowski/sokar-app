//
// Created by adam on 13.09.2021.
//

#include "IsoRangeDistanceVolumeTransform.hpp"

using namespace SokarAlg;

IsoRangeDistanceVolumeTransformPtr IsoRangeDistanceVolumeTransform::New() {

    auto self = new IsoRangeDistanceVolumeTransform;

//    self->setVolume(volume);

    return IsoRangeDistanceVolumeTransformPtr(self);
}

const Range<float> &IsoRangeDistanceVolumeTransform::getIsoRange() const {

    return iso_level;
}

void IsoRangeDistanceVolumeTransform::setIsoRange(const Range<float> &new_iso_level) {

    iso_level = new_iso_level;
}

float IsoRangeDistanceVolumeTransform::getValue(const glm::i32vec3 &position) const {

    return iso_level.distance(volume->getValue(position));
}
