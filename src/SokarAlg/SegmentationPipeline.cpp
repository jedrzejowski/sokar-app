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
        VolumePtr volume = raw_dicom_volume;
        auto result = QSharedPointer<SegmentationResult>::create();

        result->summary.timeStart = makeTimePoint();

        // volume

        if (not volume_interpolator.isNull()) {

            auto dicom_volume = DicomVolumePtr::create();
            dicom_volume->setRawDicomVolume(raw_dicom_volume);
            dicom_volume->setInterpolator(volume_interpolator);
            dicom_volume->setCubesPerMM(cubes_per_mm);

            volume = dicom_volume;
        }

        //endregion

        if (not gradient_volume.isNull()) {
            gradient_volume->setVolume(volume);
            result->gradient.description = "tak";
            volume = gradient_volume;
        } else {
            result->gradient.description = "nie";
        }

        if (use_region_growth) {
            result->region_growth.was = true;

            emit updateProgress(QObject::tr("Rozrost obszarów"), 0.f);

            auto region_growth = RegionGrowthVolume::New();
            region_growth->setVolume(volume);
            region_growth->setIsoLevel(iso_range);
            region_growth->setStartPoint(region_growth_start_point);
            volume = region_growth;

            result->region_growth.timeStart = makeTimePoint();
            region_growth->regrowth();
            result->region_growth.timeEnd = makeTimePoint();

            result->region_growth.description = QString("z punktu %1").arg(
                    glm::to_string(region_growth_start_point).c_str()
            );
        } else {
            result->region_growth.description = "nie";
        }

        if (use_empty_env) {
            auto volume_env = VolumeEnv::New();
            volume_env->setEnvValue(0.f);
            volume_env->setEnvSize(1.f);
            volume_env->setVolume(volume);
            result->volume_env.description = "tak";
            volume = volume_env;
        } else {
            result->volume_env.description = "nie";
        }

        {
            auto iso_range_volume = IsoRangeDistanceVolumeTransform::New();
            iso_range_volume->setVolume(volume);
            iso_range_volume->setIsoRange(iso_range);
            volume = iso_range_volume;
        }

        if (use_cache) {
            result->volume_interpolation_cache.was = true;

            emit updateProgress(QObject::tr("Kaszowanie interpolacji"), 0.f);

            auto cached_volume = QSharedPointer<CachedVolume>::create();
            cached_volume->setVolume(volume);
            volume = cached_volume;

            result->volume_interpolation_cache.timeStart = makeTimePoint();
            cached_volume->refreshCache();
            result->volume_interpolation_cache.timeEnd = makeTimePoint();

            result->volume_interpolation_cache.description = QString("czas kaszowania %1").arg(
                    timeRangeString(result->volume_interpolation_cache.timeStart,
                                    result->volume_interpolation_cache.timeEnd)
            );
        } else {
            result->volume_interpolation_cache.description = QString("nie");
        }

        //region segmentation

        emit updateProgress(QObject::tr("Maszerowanie"), 0.f);

        volume_segmentator->setLineInterpolator(line_interpolator);
        volume_segmentator->setVolume(volume);
        volume_segmentator->setMesh(current_mesh);

        result->segmentation.inputMesh = current_mesh;
        result->segmentation.timeStart = makeTimePoint();
        result->segmentation.outputMesh = current_mesh = volume_segmentator->execSync();
        result->segmentation.timeEnd = makeTimePoint();

        //region

        auto transform = glm::mat4(1.f);

        // przesunięcie mesha, aby osadzenie w pustej przestrzeni nie miało wpływu, na pozycje
        if (use_empty_env) {
            transform = glm::translate(transform, glm::vec3(-1.f, -1.f, -1.f));
        }

        if (not volume_interpolator.isNull()) {
            // transformata do skalowania mesha, aby zawsze był taki sam
            transform = glm::scale(transform, glm::vec3(cubes_per_mm, cubes_per_mm, cubes_per_mm));
        } else {
            // w przypadku braku interpolacji wyskaluj do wartości woksela
            transform = glm::scale(transform, raw_dicom_volume->getWokselSize());
        }


        current_mesh->applyTransform(transform);

        result->segmentation.description = QString("%1\nczas %2").arg(
                volume_segmentator->toDisplay(),
                timeRangeString(result->segmentation.timeStart, result->segmentation.timeEnd)
        );

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
        result->proposeCameraCenter = glm::vec3(volume->getSize()) / 2.f * cubes_per_mm;
        if (volume_interpolator.isNull()) {
            result->proposeCameraCenter = glm::vec3(volume->getSize()) * raw_dicom_volume->getWokselSize() / 2.f;
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

void SegmentationPipeline::setUseEmptyEnv(bool useEmptyEnv) {

    SegmentationPipeline::use_empty_env = useEmptyEnv;
}

void SegmentationPipeline::setUseRegionGrowth(bool useRegionGrowth) {

    SegmentationPipeline::use_region_growth = useRegionGrowth;
}

void SegmentationPipeline::setGrowthStartPoint(const glm::i32vec3 &growthStartPoint) {

    SegmentationPipeline::region_growth_start_point = growthStartPoint;
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

void SegmentationPipeline::setGradientVolume(const GradientVolumePtr &new_gradient_volume) {

    gradient_volume = new_gradient_volume;
}
void SegmentationPipeline::setLineInterpolator(const LineInterpolatorPtr &new_line_interpolator) {

    line_interpolator = new_line_interpolator;
}

//endregion
