//
// Created by adam on 19.09.2021.
//

#pragma once

#include "SokarAlg.hpp"
#include "Volume.hpp"

namespace SokarAlg {

    class LineInterpolator {
    protected:
        VolumeCPtr volume = nullptr;
        float iso_level;

        virtual void dicomVolumeChanged();

        LineInterpolator() = default;

    public:
        virtual ~LineInterpolator() = default;

        [[nodiscard]]
        const VolumeCPtr &getVolume() const;
        void setVolume(const VolumeCPtr &vv);

        float getIsoLevel() const;
        void setIsoLevel(float isoLevel);

        [[nodiscard]]
        virtual glm::vec3 interpolate(const Volume::Point &p1, const Volume::Point &p2) const = 0;
    };

    class HalfLineInterpolator : public LineInterpolator {
    public:
        [[nodiscard]]
        static HalfLineInterpolatorPtr New();

        [[nodiscard]]
        glm::vec3 interpolate(const Volume::Point &p1, const Volume::Point &p2) const override;
    };

    class LinearLineInterpolator : public LineInterpolator {
    public:
        [[nodiscard]]
        static LinearLineInterpolatorPtr New();

        [[nodiscard]]
        glm::vec3 interpolate(const Volume::Point &p1, const Volume::Point &p2) const override;
    };

    class SplineLineInterpolator : public LineInterpolator {
    public:
        [[nodiscard]]
        static SplineLineInterpolatorPtr New();

        [[nodiscard]]
        glm::vec3 interpolate(const Volume::Point &p1, const Volume::Point &p2) const override;
    };

}

