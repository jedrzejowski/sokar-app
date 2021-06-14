//
// Created by adam on 13.05.2021.
//

#pragma once

#include "SokarAlg.hpp"
#include "./MeshSimplificator.hpp"

namespace SokarAlg {

	class VertexClustering : public MeshSimplificator {
	private:
		glm::vec3 clusterSize;
		glm::vec3 clusterOffset;

		IndexedMeshPtr exec() override;

		VertexClustering();
	public:
		static VertexClusteringPtr New();

		[[nodiscard]]
		const glm::vec3 &getClusterSize() const;
		void setClusterSize(const glm::vec3 &clusterSize);

		[[nodiscard]]
		const glm::vec3 &getClusterOffset() const;
		void setClusterOffset(const glm::vec3 &clusterOffset);

		QString toDisplay() override;
	};
}

