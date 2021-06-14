//
// Created by adam on 03.05.2021.
//

#include <QtConcurrent/QtConcurrentRun>
#include "VolumeSegmentator.hpp"
#include "VolumeInterpolator.hpp"
#include "./IndexedMesh.hpp"

using namespace SokarAlg;

void VolumeSegmentator::execBefore() {

	if (mesh != nullptr) {
		mesh.clear();
	}

	mesh = MeshType::New();
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

const QSharedPointer<VolumeSegmentator::MeshType> &VolumeSegmentator::getMesh() const {
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

