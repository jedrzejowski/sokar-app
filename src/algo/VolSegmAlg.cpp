//
// Created by adam on 03.05.2021.
//

#include "VolSegmAlg.h"

using namespace SokarAlg;

QFuture<void> VolSegmAlg::exec() {
	triangles.clear();

	glm::vec3 size = virtualVolume->getSize();
	qDebug() << int(size.x * size.y * size.z / 4) * sizeof(Triangle);
	triangles.reserve(int(size.x * size.y * size.z / 4));
	qDebug() << triangles.capacity();

	return execAlg();
}

const VirtualVolume *VolSegmAlg::getVirtualVolume() const {
	return virtualVolume;
}

void VolSegmAlg::setVirtualVolume(const VirtualVolume *vv) {
	virtualVolume = vv;
}

const QVector<Triangle> &VolSegmAlg::getTriangles() const {
	return triangles;
}


Sokar3D::StaticMesh *VolSegmAlg::toStaticMesh() const {
	auto mesh = new Sokar3D::StaticMesh();

	for (const auto tri : triangles) {

		auto tex = glm::vec2(0);
		auto normal = glm::triangleNormal(tri.vertex0, tri.vertex1, tri.vertex2) * -1.f;

		mesh->addTriangle(
				{tri.vertex0, tex, normal},
				{tri.vertex1, tex, normal},
				{tri.vertex2, tex, normal}
		);
	}

	return mesh;
}
