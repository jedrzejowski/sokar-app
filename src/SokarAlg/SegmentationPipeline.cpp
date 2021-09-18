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
#include "IsoRangeDistanceVolumeTransform.hpp"
#include "GradientVolume.hpp"

using namespace SokarAlg;


SegmentationPipeline::SegmentationPipeline()
        : QObject(nullptr),
          volumeInterpolator(QSharedPointer<SokarAlg::NearestVolumeInterpolator>::create()) {
}

SegmentationPipelinePtr SegmentationPipeline::New() {

    return SegmentationPipelinePtr(new SegmentationPipeline);
}

QFuture<SegmentationResultCPtr> SegmentationPipeline::executePipeline() {
    // aby obiekt sam się nie rozwalił
    auto self = sharedFromThis();

    Q_ASSERT(volumeSegmentator not_eq nullptr);
    Q_ASSERT(volumeSegmentator not_eq nullptr);

    return QtConcurrent::run([self, this]() -> QSharedPointer<const SegmentationResult> {
        QMutexLocker lock(&stateMutex);

        Sokar3D::MeshPtr current_mesh = base_mesh;
        QSharedPointer<Volume> volume = rawDicomVolume;
        auto result = QSharedPointer<SegmentationResult>::create();

        result->summary.timeStart = makeTimePoint();

        // volume

        if (not volumeInterpolator.isNull()) {

            auto dicomVolume = DicomVolumePtr::create();
            dicomVolume->setRawDicomVolume(rawDicomVolume);
            dicomVolume->setInterpolator(volumeInterpolator);
            dicomVolume->setCubesPerMM(cubes_per_mm);

            volume = dicomVolume;
        }

        //endregion

        if (not gradient_volume.isNull()) {
            gradient_volume->setVolume(volume);
            volume = gradient_volume;
        }

        if (useRegionGrowth) {
            result->regionGrowth.was = true;

            emit updateProgress(QObject::tr("Rozrost obszarów"), 0.f);

            auto region_growth = RegionGrowthVolume::New();
            region_growth->setVolume(volume);
            region_growth->setIsoLevel(iso_range);
            region_growth->setStartPoint(regionGrowthStartPoint);
            volume = region_growth;

            result->regionGrowth.timeStart = makeTimePoint();
            region_growth->regrowth();
            result->regionGrowth.timeEnd = makeTimePoint();

            result->regionGrowth.description = QString("z punktu %1").arg(
                    glm::to_string(regionGrowthStartPoint).c_str()
            );
        } else {
            result->regionGrowth.description = QString("nie");
        }

        if (useEmptyEnv) {
            auto volume_env = VolumeEnv::New();
            volume_env->setEnvValue(0.f);
            volume_env->setEnvSize(1.f);
            volume_env->setVolume(volume);
            volume = volume_env;
        }

        {
            auto iso_range_volume = IsoRangeDistanceVolumeTransform::New();
            iso_range_volume->setVolume(volume);
            iso_range_volume->setIsoRange(iso_range);
            volume = iso_range_volume;
        }

        if (useInterpolationCache) {
            result->interpolationCache.was = true;

            emit updateProgress(QObject::tr("Kaszowanie interpolacji"), 0.f);

            auto cached_volume = QSharedPointer<CachedVolume>::create();
            cached_volume->setVolume(volume);
            volume = cached_volume;

            result->interpolationCache.timeStart = makeTimePoint();
            cached_volume->refreshCache();
            result->interpolationCache.timeEnd = makeTimePoint();

            result->interpolationCache.description = QString("czas kaszowania %1").arg(
                    timeRangeString(result->interpolationCache.timeStart, result->interpolationCache.timeEnd)
            );
        } else {
            result->interpolationCache.description = QString("nie");
        }

        //region segmentation

        emit updateProgress(QObject::tr("Maszerowanie"), 0.f);

        volumeSegmentator->setVolume(volume);
        volumeSegmentator->setMesh(current_mesh);

        result->segmentation.inputMesh = current_mesh;
        result->segmentation.timeStart = makeTimePoint();
        result->segmentation.outputMesh = current_mesh = volumeSegmentator->execSync();
        result->segmentation.timeEnd = makeTimePoint();

        //region

        auto transform = glm::mat4(1.f);

        // przesunięcie mesha, aby osadzenie w pustej przestrzeni nie miało wpływu, na pozycje
        if (not useEmptyEnv) {
            transform = glm::translate(transform, glm::vec3(cubes_per_mm, cubes_per_mm, cubes_per_mm));
        }

        // transformata do skalowania mesha, aby zawsze był taki sam
        transform = glm::scale(transform, glm::vec3(cubes_per_mm, cubes_per_mm, cubes_per_mm));

        current_mesh->applyTransform(transform);

        result->segmentation.description = QString("%1\nczas %2").arg(
                volumeSegmentator->toDisplay(),
                timeRangeString(result->segmentation.timeStart, result->segmentation.timeEnd)
        );

        //endregion

        //region mesh simplification


        if (meshSimplificator != nullptr) {

            emit updateProgress(QObject::tr("Upraszczanie siatki"), 0.f);

            result->simplification.inputMesh = Sokar3D::IndexedMesh::New();
            current_mesh->injectTo(result->simplification.inputMesh);

            meshSimplificator->setMesh(result->simplification.inputMesh);

            result->simplification.timeStart = makeTimePoint();
            result->simplification.outputMesh = current_mesh = meshSimplificator->execSync();
            result->simplification.timeEnd = makeTimePoint();

        } else {
            result->simplification.description = QString("nie");
        }

        //endregion

        result->summary.timeEnd = makeTimePoint();
        result->summary.mesh = current_mesh;
        result->summary.displayMesh = Sokar3D::TriangleListMesh::New();
        current_mesh->injectTo(result->summary.displayMesh);

        result->summary.description = QString("czas wykonania %1").arg(
                timeRangeString(result->summary.timeStart, result->summary.timeEnd)
        );

        result->meshColor = meshColor;
        result->proposeCameraCenter = glm::vec3(volume->getSize()) / 2.f * cubes_per_mm;
        result->proposeCameraDistance = glm::length(result->proposeCameraCenter) * 2;

        return result;
    });
}

//region getters and setters

void SegmentationPipeline::setColor(const QColor &newColor) {

    meshColor = newColor;
}

void SegmentationPipeline::setRawDicomVolume(const RawDicomVolumePtr &newRawDicomVolume) {

    rawDicomVolume = newRawDicomVolume;
}

void SegmentationPipeline::setVolumeInterpolator(const VolumeInterpolatorPtr &newVolumeInterpolator) {

    volumeInterpolator = newVolumeInterpolator;
}

void SegmentationPipeline::setVolumeSegmentator(const VolumeSegmentatorPtr &newVolumeSegmentator) {

    volumeSegmentator = newVolumeSegmentator;
}

void SegmentationPipeline::setMeshSimplificator(const MeshSimplificatorPtr &newMeshSimplificator) {

    meshSimplificator = newMeshSimplificator;
}

void SegmentationPipeline::setUseInterpolationCache(bool newUsingCache) {

    useInterpolationCache = newUsingCache;
}

void SegmentationPipeline::setUseEmptyEnv(bool useEmptyEnv) {

    SegmentationPipeline::useEmptyEnv = useEmptyEnv;
}

void SegmentationPipeline::setUseRegionGrowth(bool useRegionGrowth) {

    SegmentationPipeline::useRegionGrowth = useRegionGrowth;
}

void SegmentationPipeline::setGrowthStartPoint(const glm::i32vec3 &growthStartPoint) {

    SegmentationPipeline::regionGrowthStartPoint = growthStartPoint;
}

void SegmentationPipeline::setBaseMesh(const Sokar3D::MeshPtr &baseMesh) {

    SegmentationPipeline::base_mesh = baseMesh;
}

void SegmentationPipeline::setIsoRange(const Range<float> &isoRange) {

    iso_range = isoRange;
}

void SegmentationPipeline::setCubesPerMM(float new_cubes_per_mm) {

    cubes_per_mm = new_cubes_per_mm;
}

void SegmentationPipeline::setGradientVolume(const GradientVolumePtr &gradientVolume) {

    gradient_volume = gradientVolume;
}

//endregion
