//
// Created by adam on 30.08.2021.
//

#pragma once

#include "SokarAlg.hpp"
#include "./MeshSimplificator.hpp"

namespace SokarAlg {

    class BoundingConvexDecomposition : public MeshSimplificator {
    public:

    private:

        float alpha, delta, new_passes, vertex_ratio, target_error;

        Sokar3D::IndexedMeshPtr exec() override;

        BoundingConvexDecomposition();

    public:

        static BoundingConvexDecompositionPtr New();

        float getAlpha() const;
        void setAlpha(float alpha);
        float getDelta() const;
        void setDelta(float delta);
        float getMaxPasses() const;
        void setMaxPasses(float maxPasses);

        float getVertexRatio() const;
        void setVertexRatio(float vertexRatio);
        float getTargetError() const;
        void setTargetError(float targetError);

        QString toDisplay() override;

    };

}
