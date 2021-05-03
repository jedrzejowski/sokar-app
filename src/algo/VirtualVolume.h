//
// Created by adam on 14.03.2021.
//

#pragma once

#include "./_def.h"
#include "../_classdef.h"
#include "./Cube.h"
#include "../dicomview/scenes/sets/_sceneset.h"

namespace SokarAlg {


	class VirtualVolume : public QObject {
	Q_OBJECT
	public:

		[[nodiscard]]
		virtual glm::u32vec3 getSize() const = 0;

		[[nodiscard]]
		inline Cube getCube(const glm::u32vec3 &position) const {
			return getCube(position.x, position.y, position.z);
		}

		[[nodiscard]]
		Cube getCube(const quint32 &x, const quint32 &y, const quint32 &z) const;

		[[nodiscard]]
		inline double getValue(const glm::u32vec3 &position) const {
			return getValue(position.x, position.y, position.z);
		}

		[[nodiscard]]
		virtual float getValue(const quint32 &x, const quint32 &y, const quint32 &z) const = 0;
	};
}



