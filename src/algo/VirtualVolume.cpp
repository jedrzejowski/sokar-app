//
// Created by adam on 14.03.2021.
//

#include "./VirtualVolume.h"
#include "./VertexInterpolator.h"

using namespace SokarAlg;

Cube VirtualVolume::getCube(const glm::vec3 &position, const glm::vec3 &size) const {
	auto cube = Cube();

	// TODO dodać if'a

	cube.position[0] = position + glm::vec3(0, 0, 0);
	cube.value[0] = getValue(cube.position[0]);

	cube.position[1] = position + glm::vec3(0, size.y, 0);
	cube.value[1] = getValue(cube.position[1]);

	cube.position[2] = position + glm::vec3(size.x, size.y, 0);
	cube.value[2] = getValue(cube.position[2]);

	cube.position[3] = position + glm::vec3(size.x, 0, 0);
	cube.value[3] = getValue(cube.position[3]);

	cube.position[4] = position + glm::vec3(0, 0, size.z);
	cube.value[4] = getValue(cube.position[4]);

	cube.position[5] = position + glm::vec3(0, size.y, size.z);
	cube.value[5] = getValue(cube.position[5]);

	cube.position[6] = position + glm::vec3(size.x, size.y, size.z);
	cube.value[6] = getValue(cube.position[6]);

	cube.position[7] = position + glm::vec3(size.x, 0, size.z);
	cube.value[7] = getValue(cube.position[7]);

	return cube;
}
