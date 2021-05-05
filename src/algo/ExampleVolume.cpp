//
// Created by adam on 30.04.2021.
//

#include "ExampleVolume.h"

#include <utility>

using namespace SokarAlg;


ExampleVolume::ExampleVolume(
		const glm::u32vec3 &mySize,
		FunctionIn3D myFunction
) : mySize(mySize), myFunction(std::move(myFunction)) {}

float ExampleVolume::getValue(quint32 x, quint32 y, quint32 z) const {
	return myFunction(x, y, z);
}

glm::u32vec3 ExampleVolume::getSize() const {
	return mySize;
}

ExampleVolume *ExampleVolume::Sphere(quint32 volSize, float radius, float inVal, float outVal) {

	auto center = glm::vec3(volSize / 2, volSize / 2, volSize / 2);

	FunctionIn3D function = [=](auto x, auto y, auto z) -> float {
		return glm::distance(glm::vec3(x, y, z), center) <= radius ? inVal : outVal;
	};

	return new ExampleVolume(glm::u32vec3(volSize, volSize, volSize), function);
}

ExampleVolume *ExampleVolume::Cube(
		glm::u32vec3 volSize,
		glm::vec3 cubeSize,
		float inVal,
		float outVal,
		glm::mat4 transform
) {

	float x1, x2, y1, y2, z1, z2;

	if (transform == glm::mat4(0.f)) {
		x1 = (float(volSize.x) - cubeSize.x) / 2.f;
		x2 = (float(volSize.x) + cubeSize.x) / 2.f;

		y1 = (float(volSize.y) - cubeSize.y) / 2.f;
		y2 = (float(volSize.y) + cubeSize.y) / 2.f;

		z1 = (float(volSize.z) - cubeSize.z) / 2.f;
		z2 = (float(volSize.z) + cubeSize.z) / 2.f;
	} else {
		//TODO
		qDebug() << "TODO";
	}

	FunctionIn3D function = [=](auto x, auto y, auto z) -> float {
		return (
					   (x1 < x && x < x2) &&
					   (y1 < y && y < y2) &&
					   (z1 < z && z < z2)
			   ) ? inVal : outVal;
	};

	return new ExampleVolume(volSize, function);
}
