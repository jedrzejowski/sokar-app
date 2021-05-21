//
// Created by adam on 27.04.2021.
//

#pragma once

#include "./Sokar3D.hpp"

namespace Sokar3D {

	// https://fvcaputo.github.io/2019/02/06/memory-alignment.html
	struct SolidMaterial {
		alignas(16) glm::vec3 color;
		alignas(16) glm::vec3 specular;
		alignas(4) float shininess;
	};
}

