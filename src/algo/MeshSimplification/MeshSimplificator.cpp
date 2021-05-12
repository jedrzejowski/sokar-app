//
// Created by adam on 13.05.2021.
//

#include "./MeshSimplificator.h"
#include "3d/StaticMesh.h"

using namespace SokarAlg;

MeshSimplificator::Extrema MeshSimplificator::findExtrema(const Sokar3D::StaticMesh *mesh) {
	Extrema extrema{};

	extrema.max.x = +std::numeric_limits<float>::infinity();
	extrema.max.y = +std::numeric_limits<float>::infinity();
	extrema.max.z = +std::numeric_limits<float>::infinity();
	extrema.min.x = -std::numeric_limits<float>::infinity();
	extrema.min.y = -std::numeric_limits<float>::infinity();
	extrema.min.z = -std::numeric_limits<float>::infinity();

	for (auto vert : mesh->getVertices()) {
		extrema.max.x = std::max(vert.pos.x, extrema.max.x);
		extrema.max.y = std::max(vert.pos.y, extrema.max.y);
		extrema.max.z = std::max(vert.pos.z, extrema.max.z);
		extrema.min.x = std::min(vert.pos.x, extrema.min.x);
		extrema.min.y = std::min(vert.pos.y, extrema.min.y);
		extrema.min.z = std::min(vert.pos.z, extrema.min.z);
	}





	return extrema;
}
