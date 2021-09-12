//
// Created by adam on 13.09.2021.
//

#pragma once

#include "SokarAlg.hpp"
#include "Volume.hpp"
#include "Range.hpp"

namespace SokarAlg {
    class IsoRangeDistanceVolumeTransform : public VolumeDecorator {
    protected:
        Range<float> iso_level = {0.5f, 1.f};

        IsoRangeDistanceVolumeTransform() = default;
    public:

        [[nodiscard]]
        static IsoRangeDistanceVolumeTransformPtr New();

        [[nodiscard]]
        const Range<float> &getIsoRange() const;
        void setIsoRange(const Range<float> &isoLevel);

        [[nodiscard]]
        float getValue(const glm::i32vec3 &position) const override;
    };
}