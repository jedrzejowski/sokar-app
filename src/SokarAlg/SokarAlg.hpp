//
// Created by adam on 11.04.2021.
//

#pragma once

#include <chrono>
#include <QtGlobal>
#include "../SokarGlm.hpp"

namespace SokarAlg {

	using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

	inline TimePoint makeTimePoint(){
		return std::chrono::high_resolution_clock::now();
	}

	inline bool isZero(float num) {
		return num < SokarGlm::EPS;
	}

	inline bool areSame(float num1, float num2) {
		return std::abs(num1 - num2) < SokarGlm::EPS;
	}

	template<typename T>
	class Array3;

	class Volume;

	class wVolumeEnv;

	class VolSegmAlgorithm;

	class DicomVolume;

	class InterpolatedVolume;

	class CachedVolume;

	class ExampleVolume;

	class MarchingCubes;

	class IndexedMesh;

	// MeshSimplificator

	class MeshSimplificator;

	class VertexClustering;

	class EdgeContraction;

	class VertexDecimation;

	// VolumeInterpolator

	class VolumeInterpolator;

	class NearestVolumeInterpolator;

	class LinearVolumeInterpolator;

	class CubicVolumeInterpolator;

	class MakimaValueInterpolator;

	class SplineValueInterpolator;

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

	class SegmentationPipeline;

	struct SegmentationResult;

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
