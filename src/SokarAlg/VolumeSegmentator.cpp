//
// Created by adam on 03.05.2021.
//

#include <QtConcurrent/QtConcurrentRun>
#include "VolumeSegmentator.hpp"
#include "LineInterpolator.hpp"
#include "Sokar3D/IndexedMesh.hpp"

using namespace SokarAlg;

void VolumeSegmentator::execBefore() {

    Q_ASSERT(not volume.isNull());
    Q_ASSERT(not line_interpolator.isNull());
    Q_ASSERT(not mesh.isNull());

    line_interpolator->setVolume(volume);
}

const QSharedPointer<const Volume> &VolumeSegmentator::getVolume() const {

    return volume;
}

void VolumeSegmentator::setVolume(const VolumeCPtr &vv) {

    volume = vv;
}

void VolumeSegmentator::addTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2) {

    mesh->addTriangle(v0, v1, v2);
}

const Sokar3D::MeshPtr &VolumeSegmentator::getMesh() const {

    return mesh;
}

float VolumeSegmentator::getIsoLevel() const {

    return iso_level;
}

void VolumeSegmentator::setIsoLevel(float newIsoLevel) {

    iso_level = newIsoLevel;
}

void VolumeSegmentator::setMesh(const Sokar3D::MeshPtr &newMesh) {

    this->mesh = newMesh;
}

Volume::Point VolumeSegmentator::getPoint(const glm::i32vec3 &pos) const {

    return volume->getPoint(pos);
}

glm::vec3 VolumeSegmentator::interpolatePoint(const Volume::Point &p1, const Volume::Point &p2) {

    return line_interpolator->interpolate(p1, p2);
}

const LineInterpolatorPtr &VolumeSegmentator::getLineInterpolator() const {

    return line_interpolator;
}

void VolumeSegmentator::setLineInterpolator(const LineInterpolatorPtr &lineInterpolator) {

    line_interpolator = lineInterpolator;
}

