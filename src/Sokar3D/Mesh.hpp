//
// Created by adam on 21.08.2021.
//

#pragma once

#include "Sokar3D.hpp"

namespace Sokar3D {

    class Mesh {
    public:

        struct Face {
            glm::vec3 v0, v1, v2;
        };

        virtual ~Mesh() = default;

        virtual void addTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2) = 0;

        virtual void foreachFaces(const std::function<void(Face face)> &functor) = 0;
    };

}


