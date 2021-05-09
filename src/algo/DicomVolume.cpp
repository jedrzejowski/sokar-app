//
// Created by adam on 14.03.2021.
//

#include "./DicomVolume.h"
#include "./ValueInterpolator.h"

using namespace SokarAlg;

DicomVolume::~DicomVolume() {
	delete interpolator;
}

const Sokar::DicomSceneSet *DicomVolume::getSceneSet() const {
	return sceneSet;
}

void DicomVolume::setSceneSet(const Sokar::DicomSceneSet *newSceneSet) {
	DicomVolume::sceneSet = newSceneSet;

	dataConverter.setFile(sceneSet->getScenesVector()[0]->getGdcmFile());

	updateModel();
}

glm::vec3 DicomVolume::getSize() const {
	return size;
}

glm::u32vec3 DicomVolume::getTrueSize() const {
	return trueSize;
}

float DicomVolume::getValue(const glm::vec3 &position) const {
	glm::vec3 truePos = position / cubeSize;
	return interpolator->interpolate(truePos);
}

float DicomVolume::getTrueValue(const glm::i32vec3 &position) const {
	return glm::length(sceneSet->getScenesVector()[position.z]->getWokselValue(position.x, position.y));
}

float DicomVolume::getTrueValueSafe(const glm::i32vec3 &position) const {
	return getTrueValue({
								std::clamp(position.x, 0, trueSize.x - 1),
								std::clamp(position.y, 0, trueSize.y - 1),
								std::clamp(position.z, 0, trueSize.z - 1)
						});
}


void DicomVolume::updateModel() {

	const static gdcm::Tag TagPixelSpacing(0x0028, 0x0030);
	const static gdcm::Tag TagSliceThickness(0x0018, 0x0050);

	cubeSize = glm::vec3(1);

	if (dataConverter.hasTagWithData(TagPixelSpacing)) {

		auto spacing = dataConverter.toDecimalString(TagPixelSpacing);

		if (spacing.length() == 2) {

			cubeSize.x = spacing[1];
			cubeSize.y = spacing[0];
		}
	}

	if (dataConverter.hasTagWithData(TagSliceThickness)) {


		auto thickness = dataConverter.toDecimalString(TagSliceThickness);

		if (thickness.length() == 1) {

			cubeSize.z = thickness[0];
		}
	}

	const auto &sceneVec = sceneSet->getScenesVector();

	trueSize = glm::u32vec3(
			sceneVec[0]->getImgDimX(),
			sceneVec[0]->getImgDimY(),
			sceneVec.size()
	);

	size = glm::vec3(
			sceneVec[0]->getImgDimX(),
			sceneVec[0]->getImgDimY(),
			sceneVec.size()
	) * cubeSize;
}


ValueInterpolator *DicomVolume::getInterpolator() const {
	return interpolator;
}

void DicomVolume::setInterpolator(ValueInterpolator *newInterpolator) {
	delete interpolator;
	interpolator = newInterpolator;
	interpolator->setVolume(this);
}

const glm::vec3 &DicomVolume::getCubeSize() const {
	return cubeSize;
}