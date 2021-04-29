//
// Created by adam on 30.04.2021.
//

#pragma once

#include "./_def.h"
#include "./VirtualVolume.h"

namespace SokarAlg {


	class ExampleVolume : public VirtualVolume {
		using FunctionIn3D = std::function<float(const quint32 &x, const quint32 &y, const quint32 &z)>;

		glm::u32vec3 mySize;
		FunctionIn3D myFunction;

	public:

		ExampleVolume(const glm::u32vec3 &mySize, FunctionIn3D myFunction);

		[[nodiscard]]
		glm::u32vec3 getSize() const override;

		[[nodiscard]]
		float getValue(const quint32 &x, const quint32 &y, const quint32 &z) const override;

		static ExampleVolume Sphere(quint32 volSize, float radius, float inVal = 1.f, float outVal = 0.f);
		static ExampleVolume Cube(glm::u32vec3 volSize, glm::vec3 cubeSize, float inVal = 1.f, float outVal = 0.f, glm::mat4 transform = glm::mat4(0.f));
	};
}



