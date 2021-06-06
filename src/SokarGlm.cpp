//
// Created by adam on 03.05.2021.
//

#include "./SokarGlm.hpp"


bool SokarGlm::fastInDistance(const glm::vec3 &v1, const glm::vec3 &v2, float distance) {
	return glm::distance(v1, v2) < distance;

	return std::abs(v1.x - v2.x) < distance &&
		   std::abs(v1.y - v2.y) < distance &&
		   std::abs(v1.z - v2.z) < distance &&
		   glm::distance(v1, v2) < distance;
}
