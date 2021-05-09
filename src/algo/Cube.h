//
// Created by adam on 16.03.2021.
//
#pragma once

#include <QDebug>
#include "./_def.h"

namespace SokarAlg {

	struct Cube {
		glm::i32vec3 position[8];
		float value[8];
	};
}

QDebug operator<<(QDebug dbg, const SokarAlg::Cube &cube);