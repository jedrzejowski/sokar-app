//
// Created by adam on 18.09.2021.
//

#pragma once

#include "SokarAlg.hpp"
#include "Volume.hpp"

namespace SokarAlg {
    class GradientVolume : public VolumeDecorator {
    public:
        enum MergeStrategy {
            Sum,
            Highest,
            Average,
            RootMeanSquare,
            Median,
        };

        enum Direction {
            X = 1 << 0,
            Y = 1 << 1,
            Z = 1 << 2,
            XY = 1 << 3,
            XZ = 1 << 4,
            YZ = 1 << 5,
            XYZ = 1 << 7,
        };

    private:
        MergeStrategy merge_strategy;
        int directions;

        explicit GradientVolume();

    public:

        [[nodiscard]]
        static GradientVolumePtr New();

        void addDirections(Direction direction);

        [[nodiscard]]
        glm::i32vec3 getSize() const override;

        [[nodiscard]]
        float getValue(const glm::i32vec3 &position) const override;
    };
}


