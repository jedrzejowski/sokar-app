//
// Created by adam on 11.04.2021.
//

#pragma once

#include <chrono>
#include <QtGlobal>
#include <SokarMacro.hpp>
#include <SokarGlm.hpp>

namespace SokarAlg {

    using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

    inline TimePoint makeTimePoint() {

        return std::chrono::high_resolution_clock::now();
    }

    inline QString timeRangeString(const TimePoint &start, const TimePoint &end) {

        return QString::number(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) + " [ms]";
    }

    inline bool isZero(float num) {

        return num < SokarGlm::EPS;
    }

    inline bool areSame(float num1, float num2) {

        return std::abs(num1 - num2) < SokarGlm::EPS;
    }

    template<typename T>
    class Array3;

    template<typename Output>
    class Algorithm;

    //region Volume

    declareSharedClass(Volume)

    declareSharedClass(VolumeEnv)

    declareSharedClass(DicomVolume)

    declareSharedClass(InterpolatedVolume)

    declareSharedClass(CachedVolume)

    declareSharedClass(ExampleVolume)

    declareSharedClass(RegionGrowthVolume)

    declareSharedClass(VolumeSegmentator)

    // VolSegmAlgorithm


    declareSharedClass(VolSegmAlgorithm)

    declareSharedClass(MarchingCubes)

    // MeshSimplificator

    declareSharedClass(MeshSimplificator)

    declareSharedClass(VertexClustering)

    declareSharedClass(EdgeCollapse)

    declareSharedClass(VertexDecimation)

    // VolumeInterpolator

    declareSharedClass(VolumeInterpolator)

    declareSharedClass(NearestVolumeInterpolator)

    declareSharedClass(LinearVolumeInterpolator)

    declareSharedClass(CubicVolumeInterpolator)

    declareSharedClass(MakimaValueInterpolator)

    declareSharedClass(SplineValueInterpolator)

    // VertexInterpolator

    class VertexInterpolator;

    class NearestVertexInterpolator;

    class LinearVertexInterpolator;

    class CubicVertexInterpolator;

    class MakimaVertexInterpolator;

    class SplineVertexInterpolator;

    // others

    template<typename Unit = float>
    struct Range;

    declareSharedClass(SegmentationPipeline)

    declareSharedClass(SegmentationResult)

    void forI32space(
            const glm::i32vec3 &from,
            const glm::i32vec3 &to,
            const std::function<void(const glm::i32vec3 &pos)> &functor
    );

    std::unique_ptr<std::vector<glm::vec3>> makeI32space(
            const glm::i32vec3 &from,
            const glm::i32vec3 &to
    );
}
