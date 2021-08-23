//
// Created by adam on 21.08.2021.
//

#pragma once

#include "Sokar3D.hpp"

namespace Sokar3D {

    class Mesh {
    public:

        virtual ~Mesh() = default;

        virtual StaticMeshPtr toStaticMesh() const = 0;

        virtual void addTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2) = 0;
    };

}


