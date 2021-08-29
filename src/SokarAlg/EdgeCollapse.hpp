//
// Created by adam on 13.05.2021.
//

#pragma once

#include <boundingmesh/Decimator.h>
#include "SokarAlg.hpp"
#include "./MeshSimplificator.hpp"

namespace SokarAlg {

    class EdgeCollapse : public MeshSimplificator {
    public:
        using DecimationDirection = boundingmesh::DecimationDirection;

    private:

        DecimationDirection direction;
        float maximum_error;
        float vertex_reduction;

        Sokar3D::IndexedMeshPtr exec() override;

        EdgeCollapse();

    public:

        static EdgeCollapsePtr New();

        [[nodiscard]]
        DecimationDirection getDirection() const;
        void setDirection(DecimationDirection direction);

        [[nodiscard]]
        float getMaximumError() const;
        void setMaximumError(float new_maximum_error);

        [[nodiscard]]
        float getVertexReduction() const;
        void setVertexReduction(float vertex_reduction);

        QString toDisplay() override;

    };

}
