//
// Created by adam on 27.04.2021.
//

#pragma once

#include "./_def.h"

namespace Sokar3D {
	struct SimpleLight {
		glm::vec3 position;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};
}


