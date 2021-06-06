//
// Created by adam on 30.04.2021.
//

#include "ExampleVolume.hpp"

#include <utility>

using namespace SokarAlg;

glm::i32vec3 ExampleVolume::getSize() const {
	return size;
}

QSharedPointer<const ExampleVolume> ExampleVolume::Sphere(qint32 volSize, float radius, float inVal, float outVal) {

	// NOLINTNEXTLINE
	class SphereVolume : public ExampleVolume {
	public:
		glm::vec3 center;
		float radius;
		float inVal;
		float outVal;

		// NOLINTNEXTLINE
		float getValue(const glm::i32vec3 &position) const override {
			return SokarGlm::fastInDistance(glm::vec3(position), center, radius) ? inVal : outVal;
		}
	};

	auto volume = QSharedPointer<SphereVolume>::create();
	volume->size = glm::i32vec3(volSize, volSize, volSize);
	volume->center = glm::vec3(volSize / 2, volSize / 2, volSize / 2);
	volume->inVal = inVal;
	volume->outVal = outVal;
	volume->radius = radius;

	return volume;
}

//QSharedPointer<const ExampleVolume> ExampleVolume::Cube(
//		glm::i32vec3 volSize,
//		glm::vec3 cubeSize,
//		float inVal,
//		float outVal,
//		glm::mat4 transform
//) {
//
//	float x1, x2, y1, y2, z1, z2;
//
//	if (transform == glm::mat4(0.f)) {
//		x1 = (float(volSize.x) - cubeSize.x) / 2.f;
//		x2 = (float(volSize.x) + cubeSize.x) / 2.f;
//
//		y1 = (float(volSize.y) - cubeSize.y) / 2.f;
//		y2 = (float(volSize.y) + cubeSize.y) / 2.f;
//
//		z1 = (float(volSize.z) - cubeSize.z) / 2.f;
//		z2 = (float(volSize.z) + cubeSize.z) / 2.f;
//	} else {
//		//TODO
//		qDebug() << "TODO";
//	}
//
//	FunctionIn3D function = [=](auto pos) -> float {
//		return (
//					   (x1 < pos.x && pos.x < x2) &&
//					   (y1 < pos.y && pos.y < y2) &&
//					   (z1 < pos.z && pos.z < z2)
//			   ) ? inVal : outVal;
//	};
//
//	return new ExampleVolume(volSize, function);
//}
