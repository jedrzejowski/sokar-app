//
// Created by adam on 30.04.2021.
//

#pragma once

#include "SokarAlg.hpp"
#include "Volume.hpp"

namespace SokarAlg {


	class ExampleVolume : public Volume {
		glm::i32vec3 size;
	public:

		[[nodiscard]]
		glm::i32vec3 getSize() const override;

		static QSharedPointer<const ExampleVolume>
		Sphere(qint32 volSize, float radius, float inVal = 1.f, float outVal = 0.f);

		static QSharedPointer<const ExampleVolume>
		Cube(glm::i32vec3 volSize, glm::vec3 cubeSize, float inVal = 1.f, float outVal = 0.f,
			 glm::mat4 transform = glm::mat4(0.f));
	};
}



