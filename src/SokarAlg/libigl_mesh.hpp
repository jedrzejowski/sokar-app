//
// Created by adam on 13.12.2021.
//

#pragma once

#include "SokarMacro.hpp"
#include <Eigen/Core>

namespace SokarAlg {
    struct libigl_mesh {
        Eigen::MatrixXd verts;
        Eigen::MatrixXi faces;
    };

    using libigl_mesh_ptr = QSharedPointer<libigl_mesh>;
    using libigl_mesh_2ptr = QSharedPointer<libigl_mesh>;
}