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
		float envValue;
	public:
		explicit VolumeEnv(float envValue, VirtualVolume *child);

		[[nodiscard]]
		glm::u32vec3 getSize() const override;
		float getValue(quint32 x, quint32 y, quint32 z) const override;
	};
}

