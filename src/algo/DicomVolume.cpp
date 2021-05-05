//
// Created by adam on 14.03.2021.
//

#include "./DicomVolume.h"

using namespace SokarAlg;

const Sokar::DicomSceneSet *DicomVolume::getSceneSet() const {
	return sceneSet;
}

void DicomVolume::setSceneSet(const Sokar::DicomSceneSet *newSceneSet) {
	DicomVolume::sceneSet = newSceneSet;

	dataConverter.setFile(sceneSet->getScenesVector()[0]->getGdcmFile());

	updateModel();
}

glm::u32vec3 DicomVolume::getSize() const {
	return size;
}

float DicomVolume::getValue(quint32 x, quint32 y, quint32 z) const {
	auto point = glm::vec4(x, y, z, 1.f);


	point = inverseModel * point;


	glm::u32vec3 point32 = point;

	return glm::length(sceneSet->getScenesVector()[point32.z]->getWokselValue(point32.x, point32.y));
}

void DicomVolume::updateModel() {

	const static gdcm::Tag TagPixelSpacing(0x0028, 0x0030);
	const static gdcm::Tag TagSliceThickness(0x0018, 0x0050);

	scale = glm::vec3(1);

	if (dataConverter.hasTagWithData(TagPixelSpacing)) {

		auto spacing = dataConverter.toDecimalString(TagPixelSpacing);

		if (spacing.length() == 2) {

			scale.x = spacing[1];
			scale.y = spacing[0];
		}
	}

	if (dataConverter.hasTagWithData(TagSliceThickness)) {


		auto thickness = dataConverter.toDecimalString(TagSliceThickness);

		if (thickness.length() == 1) {

			scale.z = thickness[0];
		}
	}

	scale = scale * upScale;

	model = glm::mat4(1);
	qDebug() << "scale" << scale;
	model = glm::scale(model, scale);

	inverseModel = glm::inverse(model);

	// size

	const auto &sceneVec = sceneSet->getScenesVector();

	size = glm::vec4(
			sceneVec[0]->getImgDimX(),
			sceneVec[0]->getImgDimY(),
			sceneVec.size(),
			1.f
	) * model;
	size -= 1;
}

float DicomVolume::getUpScale() const {
	return upScale;
}

void DicomVolume::setUpScale(float upScale) {
	DicomVolume::upScale = upScale;

	updateModel();
}
