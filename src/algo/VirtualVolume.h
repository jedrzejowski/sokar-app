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
		Cube getCube(const glm::vec3 &position, const glm::vec3 &size) const;

		[[nodiscard]]
		virtual glm::vec3 getSize() const = 0;

		[[nodiscard]]
		virtual float getValue(const glm::vec3 &position) const = 0;
	};
}



