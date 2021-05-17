//
// Created by adam on 13.05.2021.
//

#pragma once

#include "../_def.h"
#include "./MeshSimplificator.h"

namespace SokarAlg {

	class VertexClustering : public MeshSimplificator {
		glm::vec3 clusterSize;
		glm::vec3 clusterOffset;

		IndexedMesh *exec(const IndexedMesh *mesh) override;
	};
}

