//
// Created by adam on 20.05.2021.
//

#pragma once

#include "SokarAlg.hpp"
#include "VolumeSegmentator.hpp"
#include "DicomVolume.hpp"
#include "VolumeEnv.hpp"
#include "SokarUi/SegmentationPipelineEditor.hpp"

namespace SokarAlg {

    struct SegmentationResult {
        QColor meshColor;
        Range<float> iso_range;

        struct {
            QString description;
            glm::vec3 woksel_size;
        } volume_interpolation;

        struct {
            bool was = false;
            TimePoint timeStart;
            TimePoint timeEnd;
            QString description;
        } volume_interpolation_cache;

        struct {
            bool was = false;
            TimePoint timeStart;
            TimePoint timeEnd;
            QString description;
        } region_growth;

        struct {
            QString description;
        } gradient;

        struct {
            Sokar3D::MeshPtr inputMesh = nullptr;
            TimePoint timeStart;
            TimePoint timeEnd;
            QString description;
            Sokar3D::MeshPtr outputMesh = nullptr;
        } segmentation;

        struct {
            QString description;
        } line_interpolation;

        struct {
            Sokar3D::IndexedMeshPtr inputMesh = nullptr;
            Sokar3D::MeshPtr outputMesh = nullptr;
            bool was = false;
            TimePoint timeStart;
            TimePoint timeEnd;
            QString description;
        } mesh_simplification;

        struct {
            QString description;
        } volume_env;

        struct {
            TimePoint timeStart;
            TimePoint timeEnd;
            QString description;
            Sokar3D::MeshPtr mesh = nullptr;
            Sokar3D::TriangleListMeshPtr displayMesh = nullptr;
        } summary;

        glm::vec3 proposeCameraCenter;
        float proposeCameraDistance;
    };

    class SegmentationPipeline : public QObject, public QEnableSharedFromThis<SegmentationPipeline> {
    Q_OBJECT

    private:
        QMutex stateMutex;

        bool use_cache = true;
        int cache_threads = 1;
        VolumeEnv::Type empty_env_type = VolumeEnv::Type::No;
        QColor meshColor = QColor("#BF4024");

        bool use_region_growth = false;
        glm::vec3 region_growth_start_point;

        Range<float> iso_range;

        float target_woksel_size = 1;
        RawDicomVolumePtr raw_dicom_volume = nullptr;
        Sokar3D::MeshPtr base_mesh = nullptr;
        VolumeInterpolatorPtr volume_interpolator = nullptr;
        VolumeSegmentatorPtr volume_segmentator = nullptr;
        MeshSimplificatorPtr mesh_simplificator = nullptr;
        GradientVolumePtr gradient_volume = nullptr;
        LineInterpolatorPtr line_interpolator = nullptr;

        SegmentationPipeline();

    public:

        static SegmentationPipelinePtr New();

        void setUseCache(bool useCache);
        void setColor(const QColor &color);
        void setRawDicomVolume(const RawDicomVolumePtr &rawDicomVolume);
        void setVolumeInterpolator(const VolumeInterpolatorPtr &volumeInterpolator);
        void setVolumeSegmentator(const QSharedPointer<VolumeSegmentator> &volumeSegmentator);
        void setMeshSimplificator(const MeshSimplificatorPtr &meshSimplificator);
        void setBaseMesh(const Sokar3D::MeshPtr &baseMesh);
        void setEmptyEnvType(VolumeEnv::Type env_type);
        void setUseRegionGrowth(bool useRegionGrowth);
        void setGrowthStartPoint(const glm::vec3 &growthStartPoint);
        void setIsoRange(const Range<float> &isoRange);
        void setTargetWokselSize(float woskel_size);
        void setGradientVolume(const GradientVolumePtr &gradientVolume);
        void setLineInterpolator(const LineInterpolatorPtr &new_line_interpolator);
        void setCacheThreads(int cacheThreads);

        QFuture<SegmentationResultCPtr> executePipeline();

    signals:
        void updateProgress(QString title, float progress);
    };
}

