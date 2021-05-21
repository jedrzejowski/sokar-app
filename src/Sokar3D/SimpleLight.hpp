//
// Created by adam on 27.04.2021.
//

#pragma once

#include "./Sokar3D.hpp"

namespace Sokar3D {

	// https://fvcaputo.github.io/2019/02/06/memory-alignment.html
	struct SimpleLight {
		alignas(16) glm::vec3 position;
		alignas(16) glm::vec3 ambient;
		alignas(16) glm::vec3 diffuse;
		alignas(16) glm::vec3 specular;
	};
}


