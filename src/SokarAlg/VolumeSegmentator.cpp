//
// Created by adam on 03.05.2021.
//

#include <QtConcurrent/QtConcurrentRun>
#include "VolumeSegmentator.hpp"
#include "VolumeInterpolator.hpp"
#include "src/Sokar3D/IndexedMesh.hpp"

using namespace SokarAlg;

void VolumeSegmentator::execBefore() {

}

const QSharedPointer<const Volume> &VolumeSegmentator::getVolume() const {

    return volume;
}

void VolumeSegmentator::setVolume(const VolumeCPtr &vv) {

    volume = vv;

    if (!volumeInterpolator.isNull() && !volume.isNull()) {
        volumeInterpolator->setVolume(volume);
    }
}

void VolumeSegmentator::addTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2) {

    mesh->addTriangle(v0, v1, v2);
}

const Sokar3D::MeshPtr &VolumeSegmentator::getMesh() const {

    return mesh;
}

Range<float> VolumeSegmentator::getIsoLevel() const {

    return isoLevel;
}

void VolumeSegmentator::setIsoLevel(Range<float> newIsoLevel) {

    isoLevel = newIsoLevel;
}


const VolumeInterpolatorPtr &VolumeSegmentator::getVolumeInterpolator() const {

    return volumeInterpolator;
}

void VolumeSegmentator::setVolumeInterpolator(const VolumeInterpolatorPtr &newVolumeInterpolator) {

    volumeInterpolator = newVolumeInterpolator;

    if (!volumeInterpolator.isNull() && !volume.isNull()) {
        volumeInterpolator->setVolume(volume);
    }
}

void VolumeSegmentator::setMesh(const Sokar3D::MeshPtr &newMesh) {

    this->mesh = newMesh;
}

Volume::Point VolumeSegmentator::getPoint(const glm::i32vec3 &pos) const {

    return volume->getPoint(pos);
}

glm::vec3 VolumeSegmentator::interpolatePoint(float value, const Volume::Point &p1, const Volume::Point &p2) {

    float mu;
    glm::vec3 p;

    if (areSame(value, p1.value)) {
        return p1.position;
    }
    if (areSame(value, p2.value)) {
        return p2.position;
    }
    if (areSame(p1.value, p2.value)) {
        return p1.position;
    }

    mu = (value - p1.value) / (p2.value - p1.value);
    p.x = float(p1.position.x) + mu * (float(p2.position.x) - float(p1.position.x));
    p.y = float(p1.position.y) + mu * (float(p2.position.y) - float(p1.position.y));
    p.z = float(p1.position.z) + mu * (float(p2.position.z) - float(p1.position.z));

    return p;
}

