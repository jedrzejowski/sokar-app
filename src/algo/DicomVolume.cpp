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
	const auto &vec = sceneSet->getScenesVector();

	qDebug() << scale;

	return glm::u32vec3(
			std::ceil(float(vec[0]->getImgDimX()) * scale.x),
			std::ceil(float(vec[0]->getImgDimX()) * scale.y),
			std::ceil(float(vec.size()) * scale.z)
	);
}

float DicomVolume::getValue(const quint32 &x, const quint32 &y, const quint32 &z) const {
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

		qDebug() << thickness.length() << thickness[0];

		if (thickness.length() == 1) {

			scale.z = thickness[0];
		}
	}

	scale = scale * upScale;

	model = glm::mat4(1);
	model = glm::scale(model, scale);

	inverseModel = glm::inverse(model);
}

float DicomVolume::getUpScale() const {
	return upScale;
}

void DicomVolume::setUpScale(float upScale) {
	DicomVolume::upScale = upScale;
}
