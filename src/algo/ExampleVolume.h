//
// Created by adam on 30.04.2021.
//

#pragma once

#include "./_def.h"
#include "./VirtualVolume.h"

namespace SokarAlg {


	class ExampleVolume : public VirtualVolume {
	Q_OBJECT
		using FunctionIn3D = std::function<float(const glm::vec3& position)>;

		glm::vec3 mySize;
		FunctionIn3D myFunction;

	public:

		ExampleVolume(const glm::vec3 &mySize, FunctionIn3D myFunction);

		[[nodiscard]]
		glm::vec3 getSize() const override;

		[[nodiscard]]
		float getValue(const glm::vec3& position) const override;

		static ExampleVolume *Sphere(float volSize, float radius, float inVal = 1.f, float outVal = 0.f);
		static ExampleVolume *Cube(glm::vec3 volSize, glm::vec3 cubeSize, float inVal = 1.f, float outVal = 0.f,
								   glm::mat4 transform = glm::mat4(0.f));
	};
}



