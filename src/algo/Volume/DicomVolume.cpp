//
// Created by adam on 14.03.2021.
//

#include "./DicomVolume.h"
#include "../DicomVolumeInterpolator.h"

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

glm::i32vec3 DicomVolume::getSize() const {
	return size;
}

glm::i32vec3 DicomVolume::getTrueSize() const {
	return trueSize;
}

float DicomVolume::getValue(const glm::i32vec3 &position) const {
	return interpolator->interpolate(
			glm::vec3(position) / wokselSize
	);
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

	wokselSize = glm::vec3(1);

	if (dataConverter.hasTagWithData(TagPixelSpacing)) {

		auto spacing = dataConverter.toDecimalString(TagPixelSpacing);

		if (spacing.length() == 2) {

			wokselSize.x = spacing[1];
			wokselSize.y = spacing[0];
		}
	}

	if (dataConverter.hasTagWithData(TagSliceThickness)) {


		auto thickness = dataConverter.toDecimalString(TagSliceThickness);

		if (thickness.length() == 1) {

			wokselSize.z = thickness[0];
		}
	}

	const auto &sceneVec = sceneSet->getScenesVector();

	wokselSize = wokselSize * cubesPerMM;

	trueSize = glm::u32vec3(
			sceneVec[0]->getImgDimX(),
			sceneVec[0]->getImgDimY(),
			sceneVec.size()
	);

	size = glm::vec3(
			sceneVec[0]->getImgDimX(),
			sceneVec[0]->getImgDimY(),
			sceneVec.size()
	) * wokselSize;
}


DicomVolumeInterpolator *DicomVolume::getInterpolator() const {
	return interpolator;
}

void DicomVolume::setInterpolator(DicomVolumeInterpolator *newInterpolator) {
	delete interpolator;
	interpolator = newInterpolator;
	interpolator->setVolume(this);
}

float DicomVolume::getCubesPerMM() const {
	return cubesPerMM;
}

void DicomVolume::setCubesPerMM(float cubesPerMm) {
	cubesPerMM = cubesPerMm;
	updateModel();
}

glm::i32vec3 DicomVolume::clamp(const glm::i32vec3 &index) const {
	return glm::clamp(index, {0, 0, 0}, trueSize - 1);
}

const glm::vec3 &DicomVolume::getWokselSize() const {
	return wokselSize;
}
