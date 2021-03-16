//
// Created by adam on 14.03.2021.
//

#include "VirtualVolume.h"

using namespace Sokar;

const DicomSceneSet *VirtualVolume::getSceneSet() const {
	return sceneSet;
}

void VirtualVolume::setSceneSet(const DicomSceneSet *sceneSet) {
	VirtualVolume::sceneSet = sceneSet;
}

u32vec3 VirtualVolume::getSize() const {
	const auto &vec = sceneSet->getScenesVector();

	return u32vec3(
			vec[0]->getImgDimX(),
			vec[0]->getImgDimX(),
			vec.size()
	);
}

float64 VirtualVolume::getValue(const uint32 &x, const uint32 &y, const uint32 &z) const {
	return sceneSet->getScenesVector()[z]->getWokselValue().x;
}

Cube VirtualVolume::getCube(const uint32 &x, const uint32 &y, const uint32 &z) const {
	auto cube = Cube();

	cube.position[0] = u32vec3(x, y, z);
	cube.value[0] = getValue(x, y, z);

	cube.position[1] = u32vec3(x, y, z);
	cube.value[1] = getValue(x, y, z);

	cube.position[2] = u32vec3(x, y, z);
	cube.value[2] = getValue(x, y, z);

	cube.position[3] = u32vec3(x, y, z);
	cube.value[3] = getValue(x, y, z);

	cube.position[4] = u32vec3(x, y, z);
	cube.value[4] = getValue(x, y, z);

	cube.position[5] = u32vec3(x, y, z);
	cube.value[5] = getValue(x, y, z);

	cube.position[6] = u32vec3(x, y, z);
	cube.value[6] = getValue(x, y, z);

	cube.position[7] = u32vec3(x, y, z);
	cube.value[7] = getValue(x, y, z);

	return cube;
}
