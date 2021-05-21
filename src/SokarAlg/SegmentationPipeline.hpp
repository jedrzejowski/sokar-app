//
// Created by adam on 20.05.2021.
//

#pragma once

#include "SokarAlg.hpp"
#include "VolumeSegmentator.hpp"
#include "DicomVolume.hpp"

namespace SokarAlg {

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
	public:

		QFuture<QSharedPointer<Sokar3D::StaticMesh>> executePipeline();

	signals:
		void updateProgress(Stage state,);

		QSharedPointer<const RawDicomVolume> rawDicomVolume;
		QSharedPointer<VolumeInterpolator> volumeInterpolator;
		QSharedPointer<VolumeSegmentator> volumeSegmentator;
		QSharedPointer<MeshSimplificator> meshSimplificator;
	};
}

