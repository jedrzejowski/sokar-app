//
// Created by adam on 11.04.2021.
//

#pragma once

#include <QDebug>
#include "./_def.h"

namespace SokarAlg {

	struct Triangle {
		glm::vec3 vertex0{};
		glm::vec3 vertex1{};
		glm::vec3 vertex2{};
	};
}


QDebug operator<<(QDebug dbg, const SokarAlg::Triangle &tri);