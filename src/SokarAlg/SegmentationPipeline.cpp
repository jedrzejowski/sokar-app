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

using namespace SokarAlg;


SegmentationPipeline::SegmentationPipeline()
		: QObject(nullptr),
		  dicomVolume(QSharedPointer<DicomVolume>::create()),
		  volumeInterpolator(QSharedPointer<SokarAlg::NearestVolumeInterpolator>::create()),
		  volumeSegmentator(QSharedPointer<SokarAlg::MarchingCubes>::create()) {
}

QFuture<QSharedPointer<const SegmentationResult>> SegmentationPipeline::executePipeline() {
	// aby obiekt sam się nie rozwalił
	auto self = sharedFromThis();

	return QtConcurrent::run([self, this]() -> QSharedPointer<const SegmentationResult> {
		QMutexLocker lock(&stateMutex);

		QSharedPointer<const Volume> volume;
		auto result = QSharedPointer<SegmentationResult>::create();
		result->timeStarted = makeTimePoint();

		// volume

		dicomVolume->setRawDicomVolume(rawDicomVolume);
		dicomVolume->setInterpolator(volumeInterpolator);

//		volume = ExampleVolume::Sphere(100,40);
		volume = dicomVolume;

		//endregion

		//region caching

		result->timePreCache = makeTimePoint();
		if (useInterpolationCache) {
			qDebug() << "caching ...";
			auto cachedVolume = QSharedPointer<CachedVolume>::create();
			cachedVolume->setVolume(volume);
			volume = cachedVolume;
		}
		result->timePostCache = makeTimePoint();

		//endregion


		if (useRegionGrowth) {
			auto regionGrowth = QSharedPointer<RegionGrowthVolume>::create();
			regionGrowth->setVolume(volume);
			regionGrowth->setIsoLevel(volumeSegmentator->getIsoLevel());
			regionGrowth->setStartPoint(regionGrowthStartPoint);

			volume = regionGrowth;

			regionGrowth->regrowth();
		}


		if (useEmptyEnv) {
			volume = QSharedPointer<VolumeEnv>::create(volume, 0.f);
		}


		//region marching
		qDebug() << "marching ...";

		volumeSegmentator->setVolume(volume);
		volumeSegmentator->execSync();

		//endregion

		auto mesh = volumeSegmentator->getMesh();

		result->mesh = mesh;
		result->meshColor = meshColor;

		result->timeEnded = makeTimePoint();

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
