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
