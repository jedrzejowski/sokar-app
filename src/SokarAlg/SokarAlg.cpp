//
// Created by adam on 20.05.2021.
//

#include "SokarAlg.hpp"


void SokarAlg::forI32space(
		const glm::i32vec3 &from,
		const glm::i32vec3 &to,
		const std::function<void(const glm::i32vec3 &pos)> &functor
) {
	glm::i32vec3 pos = from;
	for (pos.x = from.x; pos.x < to.x; ++(pos.x)) {

		for (pos.y = from.y; pos.y < to.y; ++(pos.y)) {

			for (pos.z = from.z; pos.z < to.z; ++(pos.z)) {

				functor(pos);
			}
		}
	}
}

std::unique_ptr<std::vector<glm::vec3>> SokarAlg::makeI32space(
		const glm::i32vec3 &from,
		const glm::i32vec3 &to
) {
	auto space = std::make_unique<std::vector<glm::vec3>>();

	glm::i32vec3 pos = from;

	for (pos.x = from.x; pos.x <= to.x; ++(pos.x)) {

		for (pos.y = from.y; pos.y <= to.y; ++(pos.y)) {

			for (pos.z = from.z; pos.z <= to.z; ++(pos.z)) {

				space->push_back(pos);
			}
		}
	}

	return space;
}