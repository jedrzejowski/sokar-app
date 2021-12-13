//
// Created by adam on 13.12.2021.
//

#pragma once

#include "SokarAlg.hpp"
#include "MeshSimplificator.hpp"

namespace SokarAlg {

    class EdgeCollapseSimplification : public MeshSimplificator {

        explicit EdgeCollapseSimplification();
    public:
        static EdgeCollapseSimplificationPtr New();
        QString toDisplay() override;
    protected:
        Sokar3D::IndexedMeshPtr exec() override;
    };
}



