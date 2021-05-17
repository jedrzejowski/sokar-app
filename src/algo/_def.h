//
// Created by adam on 11.04.2021.
//

#pragma once

#include <QtGlobal>
#include "../sokar_glm.h"

namespace SokarAlg {

	static const float EPS = 0.00001f;

	inline bool isZero(float num) {
		return num < EPS;
	}

	inline bool areSame(float num1, float num2) {
		return std::abs(num1 - num2) < EPS;
	}

	template<typename T>
	class Array3;

	struct Triangle;

	class VirtualVolume;

	class VolumeEnv;

	class VolSegmAlgorithm;

	class DicomVolume;

	class ExampleVolume;

	class MarchingCubes;

	// MeshSimplificator

	class MeshSimplificator;

	class VertexClustering;

	class EdgeContraction;

	class VertexDecimation;

	// DicomVolumeInterpolator

	class DicomVolumeInterpolator;

	class NearestVolumeInterpolator;

	class LinearValueInterpolator;

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
}
