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
          dicomVolume(DicomVolumePtr::create()),
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

        Sokar3D::MeshPtr currentMesh = baseMesh;
        QSharedPointer<Volume> volume;
        auto result = QSharedPointer<SegmentationResult>::create();

        result->summary.timeStart = makeTimePoint();

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

            auto regionGrowth = RegionGrowthVolume::New();
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
        volumeSegmentator->setMesh(currentMesh);

        result->segmentation.inputMesh = currentMesh;
        result->segmentation.timeStart = makeTimePoint();
        result->segmentation.outputMesh = currentMesh = volumeSegmentator->execSync();
        result->segmentation.timeEnd = makeTimePoint();

        // transformata do skalowania mesha
        auto transform = glm::mat4(1.f);
        transform = glm::scale(transform, glm::vec3(dicomVolume->getCubesPerMM()));
        currentMesh->applyTransform(transform);

        result->segmentation.description = QString("%1\nczas %2").arg(
                volumeSegmentator->toDisplay(),
                timeRangeString(result->segmentation.timeStart, result->segmentation.timeEnd)
        );
        //endregion

        //region mesh simplification


        if (meshSimplificator != nullptr) {

            emit updateProgress(QObject::tr("Upraszczanie siatki"), 0.f);

            result->simplification.inputMesh = Sokar3D::IndexedMesh::New();
            currentMesh->injectTo(result->simplification.inputMesh);

            meshSimplificator->setMesh(result->simplification.inputMesh);

            result->simplification.timeStart = makeTimePoint();
            result->simplification.outputMesh = currentMesh = meshSimplificator->execSync();
            result->simplification.timeEnd = makeTimePoint();

        } else {
            result->simplification.description = QString("nie");
        }

        //endregion

        result->summary.timeEnd = makeTimePoint();
        result->summary.mesh = currentMesh;
        result->summary.displayMesh = Sokar3D::TriangleListMesh::New();
        currentMesh->injectTo(result->summary.displayMesh);

        result->summary.description = QString("czas wykonania %1").arg(
                timeRangeString(result->summary.timeStart, result->summary.timeEnd)
        );

        result->meshColor = meshColor;
        result->proposeCameraCenter = glm::vec3(volume->getSize()) / 2.f * dicomVolume->getCubesPerMM();
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

const VolumeInterpolatorPtr &SegmentationPipeline::getVolumeInterpolator() const {

    return volumeInterpolator;
}

void SegmentationPipeline::setVolumeInterpolator(const VolumeInterpolatorPtr &newVolumeInterpolator) {

    volumeInterpolator = newVolumeInterpolator;
}

const VolumeSegmentatorPtr &SegmentationPipeline::getVolumeSegmentator() const {

    return volumeSegmentator;
}

void SegmentationPipeline::setVolumeSegmentator(const VolumeSegmentatorPtr &newVolumeSegmentator) {

    volumeSegmentator = newVolumeSegmentator;
}

const MeshSimplificatorPtr &SegmentationPipeline::getMeshSimplificator() const {

    return meshSimplificator;
}

void SegmentationPipeline::setMeshSimplificator(const MeshSimplificatorPtr &newMeshSimplificator) {

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

const Sokar3D::MeshPtr &SegmentationPipeline::getBaseMesh() const {

    return baseMesh;
}

void SegmentationPipeline::setBaseMesh(const Sokar3D::MeshPtr &baseMesh) {

    SegmentationPipeline::baseMesh = baseMesh;
}
