//
// Created by adam on 05.05.2021.
//

#pragma once

#include "./_def.h"
#include "./VirtualVolume.h"

namespace SokarAlg {
	class VolumeEnv : public VirtualVolume {
	Q_OBJECT
		const VirtualVolume *child;
		glm::vec3 envSize;
		float envValue;
	public:
		explicit VolumeEnv(VirtualVolume *child, const glm::vec3& size, float envValue);

		[[nodiscard]]
		glm::vec3 getSize() const override;
		float getValue(const glm::vec3& position) const override;
	};
}

