//
// Created by adam on 05.05.2021.
//

#pragma once

#include "../_def.h"
#include "./VirtualVolume.h"

namespace SokarAlg {
	class VolumeEnv : public VirtualVolume {
	Q_OBJECT
		const VirtualVolume *child;
		glm::i32vec3 envSize;
		float envValue;
	public:
		explicit VolumeEnv(VirtualVolume *child, const glm::i32vec3& size, float envValue);

		[[nodiscard]]
		glm::i32vec3 getSize() const override;
		float getValue(const glm::i32vec3& position) const override;
	};
}

