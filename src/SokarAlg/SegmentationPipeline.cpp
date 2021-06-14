//
// Created by adam on 20.05.2021.
//

#include <QtConcurrent/QtConcurrentRun>
#include "SegmentationPipeline.hpp"
#include "MarchingCubes.hpp"
#include "ExampleVolume.hpp"
#include "CachedVolume.hpp"
#include "VolumeEnv.hpp"
#include "RegionGrowthVolume.hpp"
#include "MeshSimplificator.hpp"

using namespace SokarAlg;


SegmentationPipeline::SegmentationPipeline()
		: QObject(nullptr),
		  dicomVolume(QSharedPointer<DicomVolume>::create()),
		  volumeInterpolator(QSharedPointer<SokarAlg::NearestVolumeInterpolator>::create()),
		  volumeSegmentator(QSharedPointer<SokarAlg::MarchingCubes>::create()) {
}

SegmentationPipelinePtr SegmentationPipeline::New() {
	return SegmentationPipelinePtr(new SegmentationPipeline);
}

QFuture<SegmentationResultCPtr> SegmentationPipeline::executePipeline() {
	// aby obiekt sam się nie rozwalił
	auto self = sharedFromThis();

	return QtConcurrent::run([self, this]() -> QSharedPointer<const SegmentationResult> {
		QMutexLocker lock(&stateMutex);

		QSharedPointer<const Volume> volume;
		auto result = QSharedPointer<SegmentationResult>::create();

		result->timeStart = makeTimePoint();

		// volume

		dicomVolume->setRawDicomVolume(rawDicomVolume);
		dicomVolume->setInterpolator(volumeInterpolator);

//		volume = ExampleVolume::Sphere(100,40);
		volume = dicomVolume;

		//endregion

		if (useInterpolationCache) {
			result->interpolationCache.was = true;

			emit updateProgress(QObject::tr("Kaszowanie interpolacji"), 0.f);

			auto cachedVolume = QSharedPointer<CachedVolume>::create();
			cachedVolume->setVolume(volume);
			volume = cachedVolume;

			result->interpolationCache.timeStart = makeTimePoint();
			cachedVolume->refreshCache();
			result->interpolationCache.timeEnd = makeTimePoint();

			result->interpolationCache.description = QString("czas kaszowania %1").arg(
					timeRangeString(result->interpolationCache.timeStart, result->interpolationCache.timeEnd)
			);
		} else {
			result->interpolationCache.description = QString("nie");
		}

		if (useRegionGrowth) {
			result->regionGrowth.was = true;

			emit updateProgress(QObject::tr("Rozrost obszarów"), 0.f);

			auto regionGrowth = QSharedPointer<RegionGrowthVolume>::create();
			regionGrowth->setVolume(volume);
			regionGrowth->setIsoLevel(volumeSegmentator->getIsoLevel());
			regionGrowth->setStartPoint(regionGrowthStartPoint);
			volume = regionGrowth;

			result->regionGrowth.timeStart = makeTimePoint();
			regionGrowth->regrowth();
			result->regionGrowth.timeEnd = makeTimePoint();

			result->regionGrowth.description = QString("z punktu %1").arg(
				glm::to_string(regionGrowthStartPoint).c_str()
			);
		} else {
			result->regionGrowth.description = QString("nie");
		}


		if (useEmptyEnv) {
			volume = QSharedPointer<VolumeEnv>::create(volume, 0.f);
		}

		//region segmentation

		emit updateProgress(QObject::tr("Maszerowanie"), 0.f);

		volumeSegmentator->setVolume(volume);
//		volumeSegmentator->setVolumeInterpolator(volumeInterpolator);

		result->segmentation.timeStart = makeTimePoint();
		result->finalMesh = result->originalMesh = volumeSegmentator->execSync();
		result->segmentation.timeEnd = makeTimePoint();

		result->segmentation.description = QString("%1\nczas %2").arg(
				volumeSegmentator->toDisplay(),
				timeRangeString(result->segmentation.timeStart, result->segmentation.timeEnd)
		);
		//endregion

		//region mesh simplification

		if (meshSimplificator != nullptr) {

			emit updateProgress(QObject::tr("Upraszczanie siatki"), 0.f);

			meshSimplificator->setMesh(IndexedMesh::fromStaticMash(result->originalMesh));

			result->simplification.timeStart = makeTimePoint();
			result->simplifiedMesh = meshSimplificator->execSync();
			result->simplification.timeEnd = makeTimePoint();

			qDebug() << result->simplifiedMesh;
			result->finalMesh = result->simplifiedMesh->toStaticMesh();
		} else {
			result->simplification.description = QString("nie");
		}

		//endregion

		result->timeEnd = makeTimePoint();

		result->description = QString("czas wykonania %1").arg(
				timeRangeString(result->timeStart, result->timeEnd)
		);

		result->meshColor = meshColor;
		result->proposeCameraCenter = glm::vec3(volume->getSize()) / 2.f;
		result->proposeCameraDistance = glm::length(result->proposeCameraCenter) * 2;

		return result;
	});
}

const QColor &SegmentationPipeline::getColor() const {
	return meshColor;
}

void SegmentationPipeline::setColor(const QColor &newColor) {
	meshColor = newColor;
}

const QSharedPointer<const RawDicomVolume> &SegmentationPipeline::getRawDicomVolume() const {
	return rawDicomVolume;
}

void SegmentationPipeline::setRawDicomVolume(const QSharedPointer<const RawDicomVolume> &newRawDicomVolume) {
	rawDicomVolume = newRawDicomVolume;
}

const QSharedPointer<DicomVolume> &SegmentationPipeline::getDicomVolume() const {
	return dicomVolume;
}

void SegmentationPipeline::setDicomVolume(const QSharedPointer<DicomVolume> &newDicomVolume) {
	dicomVolume = newDicomVolume;
}

const QSharedPointer<VolumeInterpolator> &SegmentationPipeline::getVolumeInterpolator() const {
	return volumeInterpolator;
}

void SegmentationPipeline::setVolumeInterpolator(const QSharedPointer<VolumeInterpolator> &newVolumeInterpolator) {
	volumeInterpolator = newVolumeInterpolator;
}

const QSharedPointer<VolumeSegmentator> &SegmentationPipeline::getVolumeSegmentator() const {
	return volumeSegmentator;
}

void SegmentationPipeline::setVolumeSegmentator(const QSharedPointer<VolumeSegmentator> &newVolumeSegmentator) {
	volumeSegmentator = newVolumeSegmentator;
}

const QSharedPointer<MeshSimplificator> &SegmentationPipeline::getMeshSimplificator() const {
	return meshSimplificator;
}

void SegmentationPipeline::setMeshSimplificator(const QSharedPointer<MeshSimplificator> &newMeshSimplificator) {
	meshSimplificator = newMeshSimplificator;
}

bool SegmentationPipeline::isUseInterpolationCache() const {
	return useInterpolationCache;
}

void SegmentationPipeline::setUseInterpolationCache(bool newUsingCache) {
	useInterpolationCache = newUsingCache;
}

bool SegmentationPipeline::isUseEmptyEnv() const {
	return useEmptyEnv;
}

void SegmentationPipeline::setUseEmptyEnv(bool useEmptyEnv) {
	SegmentationPipeline::useEmptyEnv = useEmptyEnv;
}

bool SegmentationPipeline::isUseRegionGrowth() const {
	return useRegionGrowth;
}

void SegmentationPipeline::setUseRegionGrowth(bool useRegionGrowth) {
	SegmentationPipeline::useRegionGrowth = useRegionGrowth;
}

const glm::i32vec3 &SegmentationPipeline::getGrowthStartPoint() const {
	return regionGrowthStartPoint;
}

void SegmentationPipeline::setGrowthStartPoint(const glm::i32vec3 &growthStartPoint) {
	SegmentationPipeline::regionGrowthStartPoint = growthStartPoint;
}
