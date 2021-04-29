//
// Created by adam on 14.03.2021.
//

#include "./DicomVolume.h"

using namespace SokarAlg;

const Sokar::DicomSceneSet *DicomVolume::getSceneSet() const {
	return sceneSet;
}

void DicomVolume::setSceneSet(const Sokar::DicomSceneSet *sceneSet) {
	DicomVolume::sceneSet = sceneSet;
}

glm::u32vec3 DicomVolume::getSize() const {
	const auto &vec = sceneSet->getScenesVector();

	return glm::u32vec3(
			vec[0]->getImgDimX(),
			vec[0]->getImgDimX(),
			vec.size()
	);
}

float DicomVolume::getValue(const quint32 &x, const quint32 &y, const quint32 &z) const {
	return sceneSet->getScenesVector()[z]->getWokselValue(x, y).x;
}