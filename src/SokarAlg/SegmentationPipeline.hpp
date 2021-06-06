//
// Created by adam on 20.05.2021.
//

#pragma once

#include "SokarAlg.hpp"
#include "VolumeSegmentator.hpp"
#include "DicomVolume.hpp"

namespace SokarAlg {

	struct SegmentationResult{
		QSharedPointer<Sokar3D::StaticMesh> mesh;
		TimePoint timeStarted;
		TimePoint timePreCache;
		TimePoint timePostCache;
		TimePoint timePreMarching;
		TimePoint timePostMarching;
		TimePoint timeEnded;

		glm::vec3 proposeCameraCenter;
		float proposeCameraDistance;
	};

	class SegmentationPipeline : public QObject {
	Q_OBJECT
	public:

		enum Stage {
			INTERPOLATION,
			MARCHING,
			MESH_SIMPLIFICATION
		};

		std::atomic<int> stepsDone = 0;
		std::atomic<int> allSteps = 0;

	private:
		bool running = false;
		Stage stage;
		bool usingCache = true;
	public:

		QFuture<SegmentationResult> executePipeline();

	signals:
		void updateProgress();

	public:
		QSharedPointer<const RawDicomVolume> rawDicomVolume;
		QSharedPointer<VolumeInterpolator> volumeInterpolator;
		QSharedPointer<VolumeSegmentator> volumeSegmentator;
		QSharedPointer<MeshSimplificator> meshSimplificator;
	};
}

