//
// Created by adam on 14.03.2021.
//

#include "VirtualVolume.h"

using namespace SokarAlg;

Cube VirtualVolume::getCube(quint32 x, quint32 y, quint32 z) const {
	auto cube = Cube();

	// TODO dodać if'a

	cube.position[0] = glm::u32vec3(x, y, z);
	cube.value[0] = getValue(x, y, z);

	cube.position[1] = glm::u32vec3(x, y + 1, z);
	cube.value[1] = getValue(x, y + 1, z);

	cube.position[2] = glm::u32vec3(x + 1, y + 1, z);
	cube.value[2] = getValue(x + 1, y + 1, z);

	cube.position[3] = glm::u32vec3(x + 1, y, z);
	cube.value[3] = getValue(x + 1, y, z);

	cube.position[4] = glm::u32vec3(x, y, z + 1);
	cube.value[4] = getValue(x, y, z + 1);

	cube.position[5] = glm::u32vec3(x, y + 1, z + 1);
	cube.value[5] = getValue(x, y + 1, z + 1);

	cube.position[6] = glm::u32vec3(x + 1, y + 1, z + 1);
	cube.value[6] = getValue(x + 1, y + 1, z + 1);

	cube.position[7] = glm::u32vec3(x + 1, y, z + 1);
	cube.value[7] = getValue(x + 1, y, z + 1);

	return cube;
}
