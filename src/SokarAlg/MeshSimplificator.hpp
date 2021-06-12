//
// Created by adam on 13.05.2021.
//

#pragma once

#include <QFuture>
#include "Sokar3D/StaticMesh.hpp"
#include "SokarAlg.hpp"
#include "IndexedMesh.hpp"
#include "Algorithm.hpp"

namespace SokarAlg {

	class MeshSimplificator : public Algorithm<IndexedMeshPtr> {
	protected:
		IndexedMeshPtr mesh;
	public:

		struct Extrema {
			glm::vec3 min;
			glm::vec3 max;
		};

		[[nodiscard]]
		Extrema findExtrema();

		const IndexedMeshPtr &getMesh() const;
		void setMesh(const IndexedMeshPtr &mesh);
	};
}



