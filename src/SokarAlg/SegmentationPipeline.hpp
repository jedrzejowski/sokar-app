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
		Sokar3D::StaticMeshPtr mesh = nullptr;
		SokarAlg::IndexedMeshPtr indexedMesh = nullptr;
		QColor meshColor;

		TimePoint timeStarted;
		TimePoint timePreCache;
		TimePoint timePostCache;
		TimePoint timePreMarching;
		TimePoint timePostMarching;
		TimePoint timeEnded;

		glm::vec3 proposeCameraCenter;
		float proposeCameraDistance;
	};

	class SegmentationPipeline : public QObject, public QEnableSharedFromThis<SegmentationPipeline> {
	Q_OBJECT

		enum Stage {
			INTERPOLATION,
			MARCHING,
			MESH_SIMPLIFICATION
		};

	private:
		QMutex stateMutex;

		bool useInterpolationCache = true;
		bool useEmptyEnv = true;
		QColor meshColor = QColor("#BF4024");

		bool useRegionGrowth = false;
		glm::i32vec3 regionGrowthStartPoint;

		QSharedPointer<const RawDicomVolume> rawDicomVolume = nullptr;
		QSharedPointer<DicomVolume> dicomVolume = nullptr;
		QSharedPointer<VolumeInterpolator> volumeInterpolator = nullptr;
		QSharedPointer<VolumeSegmentator> volumeSegmentator = nullptr;
		QSharedPointer<MeshSimplificator> meshSimplificator = nullptr;

	public:

		SegmentationPipeline();

		bool isUseInterpolationCache() const;
		void setUseInterpolationCache(bool useCache);
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
		bool isUseEmptyEnv() const;
		void setUseEmptyEnv(bool useEmptyEnv);
		bool isUseRegionGrowth() const;
		void setUseRegionGrowth(bool useRegionGrowth);
		const glm::i32vec3 &getGrowthStartPoint() const;
		void setGrowthStartPoint(const glm::i32vec3 &growthStartPoint);

		QFuture<QSharedPointer<const SegmentationResult>> executePipeline();

	signals:
		void updateProgress(SokarAlg::SegmentationPipeline::Stage stage, float progress);
	};
}

