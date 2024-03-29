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

    class MarchingCubes : public VolumeSegmentator {

        using Cube = std::array<Volume::Point, 8>;

        glm::i32vec3 cubeSize = glm::i32vec3(1);

    protected:

        Sokar3D::MeshPtr exec() override;

        MarchingCubes() = default;

    public:

        static MarchingCubesPtr New();

        QString toDisplay() override;

        [[nodiscard]]
        const glm::i32vec3 &getCubeSize() const;
        void setCubeSize(const glm::i32vec3 &cubeSize);

    private:

        [[nodiscard]]
        Cube getCube(const glm::i32vec3 &position, const glm::i32vec3 &size = glm::i32vec3(1)) const;
        quint32 marchCube(Cube cube);
    };
}

