//
// Created by adam on 19.09.2021.
//

#pragma once

#include "SokarLib/Displayable.hpp"
#include "SokarAlg.hpp"
#include "Volume.hpp"

namespace SokarAlg {

    enum LineIterationMethod {
        Bisection,
        FalsePosition
    };

    class LineInterpolator : public SokarLib::Displayable {
    protected:
        int extend_point = 0;
        VolumeCPtr volume = nullptr;
        float iso_level = 0;
        LineIterationMethod method = FalsePosition;
        int max_iter = 5;

        virtual void dicomVolumeChanged();

        LineInterpolator() = default;

    public:
        virtual ~LineInterpolator() = default;

        [[nodiscard]]
        const VolumeCPtr &getVolume() const;
        void setVolume(const VolumeCPtr &vv);

        [[nodiscard]]
        float getIsoLevel() const;
        void setIsoLevel(float isoLevel);

        [[nodiscard]]
        virtual glm::vec3 interpolate(const Volume::Point &p1, const Volume::Point &p2) const = 0;

        [[nodiscard]]
        std::vector<Volume::Point> getPoints(const Volume::Point &p1, const Volume::Point &p2) const;

        [[nodiscard]]
        int getExtendPointCount() const;
        void setExtendPointCount(int pointCount);

        [[nodiscard]]
        LineIterationMethod getMethod() const;
        void setMethod(LineIterationMethod method);

        [[nodiscard]]
        int getMaxIter() const;
        void setMaxIter(int maxIter);

    protected:
        float findRoot(int a, int b, std::function<float(float w)> &&f) const;

        [[nodiscard]]
        glm::vec3 myClamp(const glm::vec3 &output, const glm::i32vec3 &a, const glm::i32vec3 &b) const;

        [[nodiscard]]
        QString methodToString() const;
    };

    class HalfLineInterpolator : public LineInterpolator {
    public:
        [[nodiscard]]
        static HalfLineInterpolatorPtr New();

        [[nodiscard]]
        glm::vec3 interpolate(const Volume::Point &p1, const Volume::Point &p2) const override;

        QString toDisplay() override;
    };

    class LinearLineInterpolator : public LineInterpolator {
    public:
        [[nodiscard]]
        static LinearLineInterpolatorPtr New();

        [[nodiscard]]
        glm::vec3 interpolate(const Volume::Point &p1, const Volume::Point &p2) const override;

        QString toDisplay() override;
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

        QString toDisplay() override;
    };

    class SplineLineInterpolator : public LineInterpolator {
    private:
    public:
        [[nodiscard]]
        static SplineLineInterpolatorPtr New();

        [[nodiscard]]
        glm::vec3 interpolate(const Volume::Point &p1, const Volume::Point &p2) const override;

        QString toDisplay() override;
    };

}

