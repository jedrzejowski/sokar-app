//
// Created by adam on 13.05.2021.
//

#include <QtConcurrent/QtConcurrentRun>
#include "MeshSimplificator.hpp"

using namespace SokarAlg;

MeshSimplificator::Extrema MeshSimplificator::findExtrema() {
	Extrema extrema{};

	extrema.max.x = +std::numeric_limits<float>::infinity();
	extrema.max.y = +std::numeric_limits<float>::infinity();
	extrema.max.z = +std::numeric_limits<float>::infinity();
	extrema.min.x = -std::numeric_limits<float>::infinity();
	extrema.min.y = -std::numeric_limits<float>::infinity();
	extrema.min.z = -std::numeric_limits<float>::infinity();

	for (const auto& vert : mesh->getVertices()) {
		extrema.max.x = std::max(vert.x, extrema.max.x);
		extrema.max.y = std::max(vert.y, extrema.max.y);
		extrema.max.z = std::max(vert.z, extrema.max.z);
		extrema.min.x = std::min(vert.x, extrema.min.x);
		extrema.min.y = std::min(vert.y, extrema.min.y);
		extrema.min.z = std::min(vert.z, extrema.min.z);
	}

	return extrema;
}

const Sokar3D::IndexedMeshPtr &MeshSimplificator::getMesh() const {
	return mesh;
}

void MeshSimplificator::setMesh(const Sokar3D::IndexedMeshPtr &newMesh) {
	mesh = newMesh;
}
