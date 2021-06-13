//
// Created by adam on 03.05.2021.
//

#include "./SokarGlm.hpp"
#include <QtMath>

bool SokarGlm::fastInDistance(const glm::vec3 &v1, const glm::vec3 &v2, float distance) {
	return qAbs(v1.x - v2.x) < distance &&
		   qAbs(v1.y - v2.y) < distance &&
		   qAbs(v1.z - v2.z) < distance;
}
