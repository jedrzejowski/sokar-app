//
// Created by adam on 03.05.2021.
//

#include "VolumeSegmentator.hpp"
#include "./IndexedMesh.h"

using namespace SokarAlg;

QFuture<void> VolumeSegmentator::exec() {

	if (mesh != nullptr) {
		delete mesh;
	}

	mesh = new IndexedMesh();

	return execAlg();
}

const VirtualVolume *VolumeSegmentator::getVirtualVolume() const {
	return virtualVolume;
}

void VolumeSegmentator::setVirtualVolume(const VirtualVolume *vv) {
	virtualVolume = vv;
}

void VolumeSegmentator::addTriangle(const Triangle &tri) {
	mesh->addTriangle(tri.vertex0, tri.vertex1, tri.vertex2);
}

const IndexedMesh *VolumeSegmentator::getMesh() const {
	return mesh;
}
