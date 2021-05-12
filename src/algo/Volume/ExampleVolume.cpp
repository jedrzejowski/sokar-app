//
// Created by adam on 30.04.2021.
//

#include "ExampleVolume.h"

#include <utility>

using namespace SokarAlg;


ExampleVolume::ExampleVolume(
		const glm::i32vec3 &mySize,
		FunctionIn3D myFunction
) : mySize(mySize), myFunction(std::move(myFunction)) {}

float ExampleVolume::getValue(const glm::i32vec3& position) const {
	return myFunction(position);
}

glm::i32vec3 ExampleVolume::getSize() const {
	return mySize;
}

ExampleVolume *ExampleVolume::Sphere(qint32 volSize, float radius, float inVal, float outVal) {

	auto center = glm::vec3(volSize / 2, volSize / 2, volSize / 2);

	FunctionIn3D function = [=](auto pos) -> float {
		return glm::distance(pos, center) <= radius ? inVal : outVal;
	};

	return new ExampleVolume(glm::u32vec3(volSize, volSize, volSize), function);
}

ExampleVolume *ExampleVolume::Cube(
		glm::i32vec3 volSize,
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

	FunctionIn3D function = [=](auto pos) -> float {
		return (
					   (x1 < pos.x && pos.x < x2) &&
					   (y1 < pos.y && pos.y < y2) &&
					   (z1 < pos.z && pos.z < z2)
			   ) ? inVal : outVal;
	};

	return new ExampleVolume(volSize, function);
}
