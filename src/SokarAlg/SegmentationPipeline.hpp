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
		Sokar3D::StaticMeshPtr originalMesh = nullptr;
		SokarAlg::IndexedMeshPtr simplifiedMesh = nullptr;
		Sokar3D::StaticMeshPtr finalMesh = nullptr;
		QColor meshColor;

		struct {
			QString description;
		} interpolation;

		struct {
			bool was = false;
			TimePoint timeStart;
			TimePoint timeEnd;
			QString description;
		} interpolationCache;

		struct {
			bool was = false;
			TimePoint timeStart;
			TimePoint timeEnd;
			QString description;
		} regionGrowth;

		struct {
			TimePoint timeStart;
			TimePoint timeEnd;
			QString description;
		} segmentation;

		struct {
			bool was = false;
			TimePoint timeStart;
			TimePoint timeEnd;
			QString description;
		} simplification;

		TimePoint timeStart;
		TimePoint timeEnd;
		QString description;

		glm::vec3 proposeCameraCenter;
		float proposeCameraDistance;
	};

	class SegmentationPipeline : public QObject, public QEnableSharedFromThis<SegmentationPipeline> {
	Q_OBJECT

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
		MeshSimplificatorPtr meshSimplificator = nullptr;

		SegmentationPipeline();

	public:

		static SegmentationPipelinePtr New();

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
		void setMeshSimplificator(const MeshSimplificatorPtr &meshSimplificator);
		bool isUseEmptyEnv() const;
		void setUseEmptyEnv(bool useEmptyEnv);
		bool isUseRegionGrowth() const;
		void setUseRegionGrowth(bool useRegionGrowth);
		const glm::i32vec3 &getGrowthStartPoint() const;
		void setGrowthStartPoint(const glm::i32vec3 &growthStartPoint);

		QFuture<SegmentationResultCPtr> executePipeline();

	signals:
		void updateProgress(QString title, float progress);
	};
}

