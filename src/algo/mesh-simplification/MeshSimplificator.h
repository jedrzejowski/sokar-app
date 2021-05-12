//
// Created by adam on 13.05.2021.
//

#pragma once

#include <QFuture>
#include "3d/StaticMesh.h"
#include "../_def.h"

namespace SokarAlg {
	class MeshSimplificator {
	public:
		virtual QFuture<Sokar3D::StaticMesh *> simplify(const Sokar3D::StaticMesh *mesh) = 0;
	};
}



