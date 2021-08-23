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
    };

}


