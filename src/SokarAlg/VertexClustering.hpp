//
// Created by adam on 13.05.2021.
//

#pragma once

#include "SokarAlg.hpp"
#include "./MeshSimplificator.hpp"

namespace SokarAlg {

	class VertexClustering : public MeshSimplificator {
		glm::vec3 clusterSize;
		glm::vec3 clusterOffset;

		IndexedMesh *exec(const IndexedMesh *mesh) override;

	public:
		VertexClustering(const glm::vec3 &clusterSize, const glm::vec3 &clusterOffset);

		const glm::vec3 &getClusterSize() const;
		void setClusterSize(const glm::vec3 &clusterSize);
		const glm::vec3 &getClusterOffset() const;
		void setClusterOffset(const glm::vec3 &clusterOffset);
	};
}

