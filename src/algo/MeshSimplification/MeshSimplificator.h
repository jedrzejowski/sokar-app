//
// Created by adam on 13.05.2021.
//

#pragma once

#include <QFuture>
#include "3d/StaticMesh.hpp"
#include "../_def.h"

namespace SokarAlg {
	class MeshSimplificator {
	public:
		virtual QFuture<Sokar3D::StaticMesh *> simplify(const Sokar3D::IndexedStaticMesh *mesh) = 0;

		struct Extrema {
			glm::vec3 min;
			glm::vec3 max;
		};

		Extrema findExtrema(const Sokar3D::IndexedStaticMesh *mesh);
	};
}



