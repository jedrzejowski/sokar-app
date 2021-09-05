//
// Created by adam on 14.03.2021.
//

#pragma once

#include "SokarAlg.hpp"
#include "Volume.hpp"
#include "VolumeSegmentator.hpp"
#include "Sokar3D/Mesh.hpp"
#include "Range.hpp"

namespace SokarAlg {

    class MarchingTetrahedrons : public VolumeSegmentator {

        struct Tetrahedron {
            glm::i32vec3 position[4];
            float value[4];
        };

    protected:

        Sokar3D::MeshPtr exec() override;

    public:

        QString toDisplay() override;

    private:

        [[nodiscard]]
        Cube getTetrahedron(const glm::i32vec3 &position, ushort tet_num) const;
        void marchTetrahedron(Tetrahedron cube);
    };
}

