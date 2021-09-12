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

        using Tetrahedron = std::array<Volume::Point, 4>;

    protected:

        glm::i32vec3 cubeSize = glm::i32vec3(1);

        Sokar3D::MeshPtr exec() override;

        MarchingTetrahedrons() = default;

    public:

        [[nodiscard]]
        static MarchingTetrahedronsPtr New();

        QString toDisplay() override;

        [[nodiscard]]
        const glm::i32vec3 &getCubeSize() const;
        void setCubeSize(const glm::i32vec3 &cubeSize);

    private:

        void marchTetrahedron(Tetrahedron cube);
    };
}

