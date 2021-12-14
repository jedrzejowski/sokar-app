//
// Created by adam on 13.12.2021.
//

#pragma once

#include "SokarAlg.hpp"
#include "MeshSimplificator.hpp"

namespace SokarAlg {

    class EdgeCollapseSimplification : public MeshSimplificator {

        explicit EdgeCollapseSimplification();
        float vertex_reduction;

    protected:
        Sokar3D::IndexedMeshPtr exec() override;

    public:
        static EdgeCollapseSimplificationPtr New();
        QString toDisplay() override;

        float getVertexReduction() const;
        void setVertexReduction(float vertexReduction);
    };
}



