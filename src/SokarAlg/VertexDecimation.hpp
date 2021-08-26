//
// Created by adam on 13.05.2021.
//

#pragma once
#include "SokarAlg.hpp"
#include "./MeshSimplificator.hpp"


namespace SokarAlg {

    class VertexDecimation : public MeshSimplificator {
    private:

        Sokar3D::IndexedMeshPtr exec() override;

        VertexDecimation();
    public:
        static VertexDecimationPtr New();


        QString toDisplay() override;
    };
}


