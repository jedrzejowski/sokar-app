//
// Created by adam on 19.09.2021.
//

#pragma once

#include "SokarAlg.hpp"
#include "Volume.hpp"

namespace SokarAlg {

    class LineInterpolator {
    protected:
        int point_count;
        VolumeCPtr volume = nullptr;
        float iso_level = 0;

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

        [[nodiscard]]
        std::vector<Volume::Point> getPoints(const Volume::Point &p1, const Volume::Point &p2) const;

        [[nodiscard]]
        int getPointCount() const;
        void setPointCount(int pointCount);
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

    class PolynomialLineInterpolator : public LineInterpolator {
    public:
        mutable std::atomic<int> passed_interpolations;
        mutable std::atomic<int> failed_interpolations;
    public:
        [[nodiscard]]
        static PolynomialLineInterpolatorPtr New();

        [[nodiscard]]
        glm::vec3 interpolate(const Volume::Point &p1, const Volume::Point &p2) const override;

        [[nodiscard]]
        int getPointCount() const;
        void setPointCount(int pointCount);
    };

    class SplineLineInterpolator : public LineInterpolator {
    private:
    public:
        [[nodiscard]]
        static SplineLineInterpolatorPtr New();

        [[nodiscard]]
        glm::vec3 interpolate(const Volume::Point &p1, const Volume::Point &p2) const override;

    };

}

