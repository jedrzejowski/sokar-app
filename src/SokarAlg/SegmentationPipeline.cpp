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
#include "LineInterpolator.hpp"

using namespace SokarAlg;


SegmentationPipeline::SegmentationPipeline()
        : QObject(nullptr) {
}

SegmentationPipelinePtr SegmentationPipeline::New() {

    return SegmentationPipelinePtr(new SegmentationPipeline);
}

QFuture<SegmentationResultCPtr> SegmentationPipeline::executePipeline() {
    // aby obiekt sam się nie rozwalił
    auto self = sharedFromThis();

    Q_ASSERT(volume_segmentator not_eq nullptr);
    Q_ASSERT(volume_segmentator not_eq nullptr);

    return QtConcurrent::run([self, this]() -> QSharedPointer<const SegmentationResult> {
        QMutexLocker lock(&stateMutex);

        Sokar3D::MeshPtr current_mesh = base_mesh;
        VolumePtr current_volume = raw_dicom_volume;
        auto result = QSharedPointer<SegmentationResult>::create();

        result->iso_range = iso_range;
        result->summary.timeStart = makeTimePoint();

        // volume

        if (not volume_interpolator.isNull()) {

            auto dicom_volume = DicomVolumePtr::create();
            dicom_volume->setRawDicomVolume(raw_dicom_volume);
            dicom_volume->setInterpolator(volume_interpolator);
            dicom_volume->setTargetWokselSize(target_woksel_size);

            result->volume_interpolation.description = volume_interpolator->toDisplay();
            result->volume_interpolation.woksel_size = glm::vec3(target_woksel_size);

            current_volume = dicom_volume;
        } else {
            result->volume_interpolation.woksel_size = raw_dicom_volume->getWokselSize();
        }

        //endregion

        if (not gradient_volume.isNull()) {
            gradient_volume->setVolume(current_volume);
            result->gradient.description = "tak";
            current_volume = gradient_volume;
        } else {
            result->gradient.description = "nie";
        }

        if (empty_env_type != VolumeEnv::No) {
            auto volume_env = VolumeEnv::New(empty_env_type);

            float min = raw_dicom_volume->getValue(glm::i32vec3(0));
            float max = min;

            forI32space(glm::i32vec3(0), raw_dicom_volume->getSize(), [&](auto &pos) {
                auto value = raw_dicom_volume->getValue(pos);
                min = value < min ? value : min;
                max = value > max ? value : max;
            });

            volume_env->setEnvValue(min - (std::abs(max - min) * 1000));
//            volume_env->setEnvValue(0.f);
            volume_env->setEnvSize(1.f);
            volume_env->setVolume(current_volume);
            result->volume_env.description = "tak";
            current_volume = volume_env;
        } else {
            result->volume_env.description = "nie";
        }

        if (use_cache) {
            result->volume_interpolation_cache.was = true;

            emit updateProgress(QObject::tr("Kaszowanie interpolacji"), 0.f);

            auto cached_volume = QSharedPointer<CachedVolume>::create();
            cached_volume->setVolume(current_volume);
            cached_volume->setThreadsCount(cache_threads);
            current_volume = cached_volume;

            result->volume_interpolation_cache.timeStart = makeTimePoint();
            cached_volume->refreshCache();
            result->volume_interpolation_cache.timeEnd = makeTimePoint();

            result->volume_interpolation_cache.description = "tak";
        } else {
            result->volume_interpolation_cache.description = "nie";
        }

        if (use_region_growth) {
            result->region_growth.was = true;

            emit updateProgress(QObject::tr("Rozrost obszarów"), 0.f);

            auto region_growth = RegionGrowthVolume::New();
            region_growth->setVolume(current_volume);
            region_growth->setIsoLevel(iso_range);

            auto dicom_size_in_mm = raw_dicom_volume->getWokselSize() * glm::vec3(raw_dicom_volume->getSize());
            glm::vec3 start_point = glm::vec3(current_volume->getSize()) *
                                    (region_growth_start_point / dicom_size_in_mm);

            region_growth->setStartPoint(start_point);
            current_volume = region_growth;

            result->region_growth.timeStart = makeTimePoint();
            region_growth->regrowth();
            result->region_growth.timeEnd = makeTimePoint();
            result->region_growth.description = region_growth->toDisplay();

        } else {
            result->region_growth.description = "nie";
        }

        {
            auto iso_range_volume = IsoRangeDistanceVolumeTransform::New();
            iso_range_volume->setVolume(current_volume);
            iso_range_volume->setIsoRange(iso_range);
            current_volume = iso_range_volume;
        }

        //region segmentation

        emit updateProgress(QObject::tr("Maszerowanie"), 0.f);

        volume_segmentator->setLineInterpolator(line_interpolator);
        volume_segmentator->setVolume(current_volume);
        volume_segmentator->setMesh(current_mesh);

        result->segmentation.inputMesh = current_mesh;
        result->segmentation.timeStart = makeTimePoint();
        result->segmentation.outputMesh = current_mesh = volume_segmentator->execSync();
        result->segmentation.timeEnd = makeTimePoint();
        result->segmentation.description = volume_segmentator->toDisplay();

        //region

        auto transform = glm::mat4(1.f);

        // przesunięcie mesha, aby osadzenie w pustej przestrzeni nie miało wpływu, na pozycje
        if (empty_env_type != VolumeEnv::No) {
            transform = glm::translate(transform, glm::vec3(-target_woksel_size));
        }

        if (not volume_interpolator.isNull()) {
            // transformata do skalowania mesha, aby zawsze był taki sam
            transform = glm::scale(transform, glm::vec3(target_woksel_size, target_woksel_size, target_woksel_size));
        } else {
            // w przypadku braku interpolacji wyskaluj do wartości woksela
            transform = glm::scale(transform, raw_dicom_volume->getWokselSize());
        }


        current_mesh->applyTransform(transform);

        result->segmentation.description = volume_segmentator->toDisplay();

        //endregion

        //region mesh simplification


        if (mesh_simplificator != nullptr) {

            emit updateProgress(QObject::tr("Upraszczanie siatki"), 0.f);

            result->mesh_simplification.inputMesh = Sokar3D::IndexedMesh::New();
            current_mesh->injectTo(result->mesh_simplification.inputMesh);

            mesh_simplificator->setMesh(result->mesh_simplification.inputMesh);

            result->mesh_simplification.timeStart = makeTimePoint();
            result->mesh_simplification.outputMesh = current_mesh = mesh_simplificator->execSync();
            result->mesh_simplification.timeEnd = makeTimePoint();
            result->mesh_simplification.description = mesh_simplificator->toDisplay();

        } else {
            result->mesh_simplification.description = QString("nie");
        }

        //endregion

        result->line_interpolation.description = line_interpolator->toDisplay();

        result->summary.timeEnd = makeTimePoint();
        result->summary.mesh = current_mesh;
        result->summary.displayMesh = Sokar3D::TriangleListMesh::New();
        current_mesh->injectTo(result->summary.displayMesh);

        result->summary.description = QString("czas wykonania %1").arg(
                timeRangeString(result->summary.timeStart, result->summary.timeEnd)
        );

        result->meshColor = meshColor;
        result->proposeCameraCenter = glm::vec3(current_volume->getSize()) / 2.f * target_woksel_size;
        if (volume_interpolator.isNull()) {
            result->proposeCameraCenter =
                    glm::vec3(current_volume->getSize()) * raw_dicom_volume->getWokselSize() / 2.f;
        }
        result->proposeCameraDistance = glm::length(result->proposeCameraCenter) * 2;

        return result;
    });
}

//region getters and setters

void SegmentationPipeline::setColor(const QColor &newColor) {

    meshColor = newColor;
}

void SegmentationPipeline::setRawDicomVolume(const RawDicomVolumePtr &newRawDicomVolume) {

    raw_dicom_volume = newRawDicomVolume;
}

void SegmentationPipeline::setVolumeInterpolator(const VolumeInterpolatorPtr &newVolumeInterpolator) {

    volume_interpolator = newVolumeInterpolator;
}

void SegmentationPipeline::setVolumeSegmentator(const VolumeSegmentatorPtr &newVolumeSegmentator) {

    volume_segmentator = newVolumeSegmentator;
}

void SegmentationPipeline::setMeshSimplificator(const MeshSimplificatorPtr &newMeshSimplificator) {

    mesh_simplificator = newMeshSimplificator;
}

void SegmentationPipeline::setUseCache(bool newUsingCache) {

    use_cache = newUsingCache;
}

void SegmentationPipeline::setEmptyEnvType(VolumeEnv::Type env_type) {

    SegmentationPipeline::empty_env_type = env_type;
}

void SegmentationPipeline::setUseRegionGrowth(bool useRegionGrowth) {

    SegmentationPipeline::use_region_growth = useRegionGrowth;
}

void SegmentationPipeline::setGrowthStartPoint(const glm::vec3 &growthStartPoint) {

    SegmentationPipeline::region_growth_start_point = growthStartPoint;
}

void SegmentationPipeline::setBaseMesh(const Sokar3D::MeshPtr &baseMesh) {

    SegmentationPipeline::base_mesh = baseMesh;
}

void SegmentationPipeline::setIsoRange(const Range<float> &isoRange) {

    iso_range = isoRange;
}

void SegmentationPipeline::setTargetWokselSize(float new_woskel_size) {

    target_woksel_size = new_woskel_size;
}

void SegmentationPipeline::setGradientVolume(const GradientVolumePtr &new_gradient_volume) {

    gradient_volume = new_gradient_volume;
}

void SegmentationPipeline::setLineInterpolator(const LineInterpolatorPtr &new_line_interpolator) {

    line_interpolator = new_line_interpolator;
}

void SegmentationPipeline::setCacheThreads(int cacheThreads) {

    cache_threads = cacheThreads;
}

//endregion
