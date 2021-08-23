//
// Created by adam on 13.05.2021.
//

#pragma once

#include <QFuture>
#include "Sokar3D/StaticMesh.hpp"
#include "SokarAlg.hpp"
#include "src/Sokar3D/IndexedMesh.hpp"
#include "Algorithm.hpp"

namespace SokarAlg {

    class MeshSimplificator : public Algorithm<Sokar3D::IndexedMeshPtr> {
    protected:
        Sokar3D::IndexedMeshPtr mesh;
    public:

        struct Extrema {
            glm::vec3 min;
            glm::vec3 max;
        };

        [[nodiscard]]
        Extrema findExtrema();

        const Sokar3D::IndexedMeshPtr &getMesh() const;
        void setMesh(const Sokar3D::IndexedMeshPtr &mesh);
    };
}



