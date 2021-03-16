//
// Created by adam on 14.03.2021.
//

#pragma once

#include "../_classdef.h"
#include "../dicomview/scenes/sets/_sceneset.h"

namespace Sokar {

	struct Cube {
		u32vec2 position[8];
		float64 value[8];
	};

	class VirtualVolume {


		const DicomSceneSet *sceneSet = nullptr;

	public:

		[[nodiscard]]
		const DicomSceneSet *getSceneSet() const;

		void setSceneSet(const DicomSceneSet *sceneSet);

		[[nodiscard]]
		u32vec3 getSize() const;

		[[nodiscard]]
		inline Cube getCube(const u32vec3 &position) const {
			return getCube(position.x, position.y, position.z);
		}

		[[nodiscard]]
		Cube getCube(const uint32 &x, const uint32 &y, const uint32 &z) const;

		[[nodiscard]]
		inline float64 getValue(const u32vec3 &position) const {
			return getValue(position.x, position.y, position.z);
		}

		[[nodiscard]]
		float64 getValue(const uint32 &x, const uint32 &y, const uint32 &z) const;
	};
}



