//
// Created by adam on 03.05.2021.
//

#include "VolSegmAlg.h"
#include "./IndexedMesh.h"

using namespace SokarAlg;

QFuture<void> VolSegmAlg::exec() {

	if (mesh != nullptr) {
		delete mesh;
	}

	mesh = new IndexedMesh();

	return execAlg();
}

const VirtualVolume *VolSegmAlg::getVirtualVolume() const {
	return virtualVolume;
}

void VolSegmAlg::setVirtualVolume(const VirtualVolume *vv) {
	virtualVolume = vv;
}

void VolSegmAlg::addTriangle(const Triangle &tri) {
	mesh->addTriangle(tri.vertex0, tri.vertex1, tri.vertex2);
}

const IndexedMesh *VolSegmAlg::getMesh() const {
	return mesh;
}
