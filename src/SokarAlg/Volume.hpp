//
// Created by adam on 14.03.2021.
//

#pragma once

#include "SokarAlg.hpp"
#include "dicomview/scenes/sets/_sceneset.h"

namespace SokarAlg {


	class Volume {
	public:

		[[nodiscard]]
		virtual glm::i32vec3 getSize() const = 0;

		[[nodiscard]]
		virtual float getValue(const glm::i32vec3 &position) const = 0;

		[[nodiscard]]
		float getValueSafe(const glm::i32vec3 &position) const;

		[[nodiscard]]
		glm::i32vec3 clamp(const glm::i32vec3 &size) const;
	};
}



