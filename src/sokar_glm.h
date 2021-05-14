//
// Created by adam on 03.05.2021.
//

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/normal.hpp>

#include <QDebug>

template<int L, typename T, glm::qualifier Q>
inline QDebug operator<<(QDebug dbg, const glm::vec<L, T, Q> &vec) {
	return dbg << glm::to_string(vec).c_str();
}

template<int L, int R, typename T, glm::qualifier Q>
inline QDebug operator<<(QDebug dbg, const glm::mat<L, R, T, Q> &mat) {
	return dbg << glm::to_string(mat).c_str();
}

namespace SokarGlm {

	inline void foreachSpace(
			const glm::i32vec3 &from,
			const glm::i32vec3 &to,
			std::function<void(glm::i32vec3 pos)> functor
	) {
		glm::i32vec3 pos = from;
		for (; pos.x <= to.x; ++pos.x) {
			for (; pos.y <= to.y; ++pos.y) {
				for (; pos.z <= to.z; ++pos.z) {
					qDebug() << pos;
					functor(pos);
				}
			}
		}
	}
}
