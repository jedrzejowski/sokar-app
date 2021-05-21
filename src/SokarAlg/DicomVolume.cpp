//
// Created by adam on 14.03.2021.
//

#include "./DicomVolume.hpp"
#include "VolumeInterpolator.hpp"

using namespace SokarAlg;

//region RawDicomVolume

const Sokar::DicomSceneSet *RawDicomVolume::getSceneSet() const {
	return sceneSet;
}

void RawDicomVolume::setSceneSet(const Sokar::DicomSceneSet *newSceneSet) {
	sceneSet = newSceneSet;

	dataConverter.setFile(sceneSet->getScenesVector()[0]->getGdcmFile());
}

glm::vec3 RawDicomVolume::getWokselSize() const {


	const static gdcm::Tag TagPixelSpacing(0x0028, 0x0030);
	const static gdcm::Tag TagSliceThickness(0x0018, 0x0050);

	glm::vec3 wokselSize = glm::vec3(1);

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

	return wokselSize;
}

glm::i32vec3 RawDicomVolume::getSize() const {
	const auto &sceneVec = sceneSet->getScenesVector();

	return glm::i32vec3(
			sceneVec[0]->getImgDimX(),
			sceneVec[0]->getImgDimY(),
			sceneVec.size()
	);
}

float RawDicomVolume::getValue(const glm::i32vec3 &position) const {
	return glm::length(sceneSet->getScenesVector()[position.z]->getWokselValue(position.x, position.y));;
}

//endregion

//region DicomVolume: getters & setters

float DicomVolume::getCubesPerMM() const {
	return cubesPerMM;
}

void DicomVolume::setCubesPerMM(float cubesPerMm) {
	cubesPerMM = cubesPerMm;
	update();
}

const QSharedPointer<const RawDicomVolume> &DicomVolume::getRawDicomVolume() const {
	return rawDicomVolume;
}

void DicomVolume::setRawDicomVolume(const QSharedPointer<const RawDicomVolume> &newRawDicomVolume) {
	rawDicomVolume = newRawDicomVolume;
	setVolume(rawDicomVolume);
	update();
}

//endregion


void DicomVolume::update() {
	auto wokselSize = glm::vec3(1.f);
	if (rawDicomVolume != nullptr) {
		wokselSize = rawDicomVolume->getWokselSize();
	}

	wokselSize = wokselSize / cubesPerMM;

	setSpaceTranslator([wokselSize](auto &position) {
		return position / wokselSize;
	});
}

glm::i32vec3 DicomVolume::getSize() const {
	return glm::vec3(rawDicomVolume->getSize()) * rawDicomVolume->getWokselSize();
}

