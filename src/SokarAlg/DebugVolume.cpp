//
// Created by adam on 06.02.2022.
//

#include "DebugVolume.hpp"

using namespace SokarAlg;


DebugCubeVolumePtr DebugCubeVolume::create(const RawDicomVolumePtr &raw_dicom_volume) {

    auto self = new DebugCubeVolume();
    self->raw_dicom_volume = raw_dicom_volume;
    self->size = raw_dicom_volume->getSize();
    self->from = self->size / 5;
    self->to = self->size / 5 * 4;

    return DebugCubeVolumePtr(self);
}

glm::i32vec3 DebugCubeVolume::getSize() const {

    return size;
}

float DebugCubeVolume::getValue(const glm::i32vec3 &position) const {

    return (
                   position.x >= from.x and
                   position.x <= to.x and
                   position.y >= from.y and
                   position.y <= to.y and
                   position.z >= from.z and
                   position.z <= to.z
           ) ? 1000.f : 0.f;
}

glm::vec3 DebugCubeVolume::getWokselSize() const {

    return raw_dicom_volume->getWokselSize();
}

//

DebugSphereVolumePtr DebugSphereVolume::create(const RawDicomVolumePtr &raw_dicom_volume) {

    auto self = new DebugSphereVolume();
    self->raw_dicom_volume = raw_dicom_volume;
    self->size = raw_dicom_volume->getSize();
    self->center = glm::vec3(self->size) / 2.f;
    self->radius = std::fmin(std::fmin(self->size.x, self->size.y), self->size.z) / 3.f;
    self->radius = std::fmax(std::fmax(self->size.x, self->size.y), self->size.z) / 3.f;

    return DebugSphereVolumePtr(self);
}

glm::i32vec3 DebugSphereVolume::getSize() const {

    return size;
}

float DebugSphereVolume::getValue(const glm::i32vec3 &position) const {

    return glm::distance(glm::vec3(position), center) < radius ? 1000.f : 0.f;
}

glm::vec3 DebugSphereVolume::getWokselSize() const {

    return raw_dicom_volume->getWokselSize();
}
