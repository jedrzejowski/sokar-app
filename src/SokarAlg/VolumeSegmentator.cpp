//
// Created by adam on 03.05.2021.
//

#include "VolumeSegmentator.hpp"
#include "./IndexedMesh.hpp"

using namespace SokarAlg;

QFuture<void> VolumeSegmentator::exec() {

	if (mesh != nullptr) {
		delete mesh;
	}

	mesh = new IndexedMesh();

	return execAlg();
}

const Volume *VolumeSegmentator::getVolume() const {
	return volume;
}

void VolumeSegmentator::setVolume(const Volume *vv) {
	volume = vv;
}

void VolumeSegmentator::addTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2) {
	mesh->addTriangle(v0, v1, v2);
}

const IndexedMesh *VolumeSegmentator::getMesh() const {
	return mesh;
}
