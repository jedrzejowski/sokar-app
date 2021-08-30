//
// Created by adam on 21.08.2021.
//

#pragma once

#include "Sokar3D.hpp"
#include "SokarLib/WavefrontObjBuilder.hpp"

namespace Sokar3D {

    class Mesh {
    public:

        struct Face {
            glm::vec3 v0, v1, v2;
        };

        virtual ~Mesh() = default;

        virtual void addTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2) = 0;

        virtual void foreachFaces(const std::function<void(Face face)> &functor) const = 0;

        virtual void dump2wavefront(SokarLib::WavefrontObjBuilder &builder) const = 0;
    };

}


