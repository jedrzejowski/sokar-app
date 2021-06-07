//
// Created by adam on 20.05.2021.
//

#pragma once

#include "SokarAlg.hpp"
#include "VolumeSegmentator.hpp"
#include "DicomVolume.hpp"
#include "DicomVolume.hpp"
#include "SokarUi/SegmentationPipelineEditor.hpp"

namespace SokarAlg {

	struct SegmentationResult {
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
		friend SokarUi::SegmentationPipelineEditor;
	public:

		SegmentationPipeline();

		enum Stage {
			INTERPOLATION,
			MARCHING,
			MESH_SIMPLIFICATION
		};

		QAtomicInt stepsDone = 0;
		QAtomicInt allSteps = 0;

	private:
		bool running = false;
		Stage stage;
		bool usingCache = true;

		float cubesPerMm;
		QColor color = QColor("#BF4024");
		QSharedPointer<const RawDicomVolume> rawDicomVolume = nullptr;
		QSharedPointer<VolumeInterpolator> volumeInterpolator = nullptr;
		QSharedPointer<VolumeSegmentator> volumeSegmentator = nullptr;
		QSharedPointer<MeshSimplificator> meshSimplificator = nullptr;

		QFuture<QSharedPointer<const SegmentationResult>> executePipeline();

	signals:
		void updateProgress();
	};
}

