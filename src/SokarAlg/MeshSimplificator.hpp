//
// Created by adam on 13.05.2021.
//

#pragma once

#include <QFuture>
#include "Sokar3D/StaticMesh.hpp"
#include "SokarAlg.hpp"
#include "IndexedMesh.hpp"

namespace SokarAlg {
	class MeshSimplificator {


		virtual IndexedMesh *exec(const IndexedMesh *mesh) = 0;
	public:
		QFuture<IndexedMesh *> simplify(const IndexedMesh *mesh);

		struct Extrema {
			glm::vec3 min;
			glm::vec3 max;
		};

		Extrema findExtrema(const IndexedMesh *mesh);
	};
}



