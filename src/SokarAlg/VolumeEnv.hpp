//
// Created by adam on 05.05.2021.
//

#pragma once

#include "SokarAlg.hpp"
#include "Volume.hpp"

namespace SokarAlg {

	class VolumeEnv : public Volume {

		QSharedPointer<const Volume> child;
		glm::i32vec3 envSize;
		float envValue;

	public:
		explicit VolumeEnv(const QSharedPointer<const Volume> &child, float envValue);

		[[nodiscard]]
		glm::i32vec3 getSize() const override;
		[[nodiscard]]
		float getValue(const glm::i32vec3 &position) const override;
	};
}

