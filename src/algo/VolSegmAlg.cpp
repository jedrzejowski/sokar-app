//
// Created by adam on 03.05.2021.
//

#include "VolSegmAlg.h"

using namespace SokarAlg;

QFuture<void> VolSegmAlg::exec() {

	if (staticMesh != nullptr) {
		delete staticMesh;
	}

	staticMesh = new Sokar3D::StaticMesh();

	return execAlg();
}

const VirtualVolume *VolSegmAlg::getVirtualVolume() const {
	return virtualVolume;
}

void VolSegmAlg::setVirtualVolume(const VirtualVolume *vv) {
	virtualVolume = vv;
}

Sokar3D::StaticMesh *VolSegmAlg::dumpStaticMesh() {
	auto mesh = staticMesh;
	staticMesh = nullptr;
	return mesh;
}

void VolSegmAlg::addTriangle(const Triangle &tri) {
	auto tex = glm::vec2(0);
	auto normal = glm::triangleNormal(tri.vertex0, tri.vertex1, tri.vertex2) * -1.f;

	staticMesh->addTriangle(
			{tri.vertex0, tex, normal},
			{tri.vertex1, tex, normal},
			{tri.vertex2, tex, normal}
	);
}
