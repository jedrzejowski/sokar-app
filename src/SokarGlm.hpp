//
// Created by adam on 03.05.2021.
//

#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/normal.hpp>

#include <QDebug>

namespace SokarGlm {

    static const float EPS = 0.00001f;
    static const float EPS_64 = 0.00001;
    static const float EPS_32 = 0.00001f;

    [[nodiscard]]
    bool fastInDistance(const glm::vec3 &v1, const glm::vec3 &v2, float distance = EPS);
}

template<int L, typename T, glm::qualifier Q>
inline QDebug operator<<(QDebug dbg, const glm::vec<L, T, Q> &vec) {

    return dbg << glm::to_string(vec).c_str();
}

template<int L, int R, typename T, glm::qualifier Q>
inline QDebug operator<<(QDebug dbg, const glm::mat<L, R, T, Q> &mat) {

    return dbg << glm::to_string(mat).c_str();
}

template<typename T, glm::qualifier Q>
inline bool operator<(const glm::vec<3, T, Q> &vecL, const glm::vec<3, T, Q> &vecR) {

    return vecL.x < vecR.x && vecL.y < vecR.y && vecL.z < vecR.z;
}


