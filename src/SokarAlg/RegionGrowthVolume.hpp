//
// Created by adam on 09.06.2021.
//

#pragma once

#include "SokarAlg.hpp"
#include "Volume.hpp"
#include "Array3.hpp"
#include "Range.hpp"

namespace SokarAlg {
    class RegionGrowthVolume : public VolumeDecorator {

        glm::i32vec3 startPoint = glm::i32vec3(0);
        float maskValue = 0.0f;

        Array3<quint8> mask;

        Range<float> isoLevel;

        [[nodiscard]]
        bool isVisited(const glm::i32vec3 &position) const;

    public:

        static RegionGrowthVolumePtr New();

        [[nodiscard]]
        float getValue(const glm::i32vec3 &position) const override;

        [[nodiscard]]
        float getMaskedValue(const glm::i32vec3 &position) const;

        [[nodiscard]]
        float getUnMaskedValue(const glm::i32vec3 &position) const;

        [[nodiscard]]
        const glm::i32vec3 &getStartPoint() const;
        void setStartPoint(const glm::i32vec3 &startPoint);

        [[nodiscard]]
        float getMaskValue() const;
        void setMaskValue(float maskValue);

        [[nodiscard]]
        const Range<float> &getIsoLevel() const;
        void setIsoLevel(const Range<float> &isoLevel);


        void regrowth();
    };
}


