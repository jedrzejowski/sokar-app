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

		enum Stage {
			INTERPOLATION,
			MARCHING,
			MESH_SIMPLIFICATION
		};

	private:
		bool running = false;
		Stage stage;
		bool usingCache = true;

		QAtomicInt stepsDone = 0;
		QAtomicInt allSteps = 0;

		QColor color = QColor("#BF4024");
		QSharedPointer<const RawDicomVolume> rawDicomVolume = nullptr;
		QSharedPointer<DicomVolume> dicomVolume = nullptr;
		QSharedPointer<VolumeInterpolator> volumeInterpolator = nullptr;
		QSharedPointer<VolumeSegmentator> volumeSegmentator = nullptr;
		QSharedPointer<MeshSimplificator> meshSimplificator = nullptr;

		QFuture<QSharedPointer<const SegmentationResult>> executePipeline();

	public:

		SegmentationPipeline();

		const QColor &getColor() const;
		void setColor(const QColor &color);
		const QSharedPointer<const RawDicomVolume> &getRawDicomVolume() const;
		void setRawDicomVolume(const QSharedPointer<const RawDicomVolume> &rawDicomVolume);
		const QSharedPointer<DicomVolume> &getDicomVolume() const;
		void setDicomVolume(const QSharedPointer<DicomVolume> &dicomVolume);
		const QSharedPointer<VolumeInterpolator> &getVolumeInterpolator() const;
		void setVolumeInterpolator(const QSharedPointer<VolumeInterpolator> &volumeInterpolator);
		const QSharedPointer<VolumeSegmentator> &getVolumeSegmentator() const;
		void setVolumeSegmentator(const QSharedPointer<VolumeSegmentator> &volumeSegmentator);
		const QSharedPointer<MeshSimplificator> &getMeshSimplificator() const;
		void setMeshSimplificator(const QSharedPointer<MeshSimplificator> &meshSimplificator);

	signals:
		void updateProgress();
	};
}

