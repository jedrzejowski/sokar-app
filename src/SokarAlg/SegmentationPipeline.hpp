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
            Sokar3D::MeshPtr inputMesh = nullptr;
            TimePoint timeStart;
            TimePoint timeEnd;
            QString description;
            Sokar3D::MeshPtr outputMesh = nullptr;
        } segmentation;

        struct {
            Sokar3D::IndexedMeshPtr inputMesh = nullptr;
            Sokar3D::MeshPtr outputMesh = nullptr;
            bool was = false;
            TimePoint timeStart;
            TimePoint timeEnd;
            QString description;
        } simplification;

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

        bool useInterpolationCache = true;
        bool useEmptyEnv = true;
        QColor meshColor = QColor("#BF4024");

        bool useRegionGrowth = false;
        glm::i32vec3 regionGrowthStartPoint;

        Range<float> iso_range;

        float cubes_per_mm = 1;
        RawDicomVolumePtr rawDicomVolume = nullptr;
        Sokar3D::MeshPtr base_mesh = nullptr;
        VolumeInterpolatorPtr volumeInterpolator = nullptr;
        VolumeSegmentatorPtr volumeSegmentator = nullptr;
        MeshSimplificatorPtr meshSimplificator = nullptr;
        GradientVolumePtr gradient_volume = nullptr;

        SegmentationPipeline();

    public:

        static SegmentationPipelinePtr New();

        void setUseInterpolationCache(bool useCache);
        void setColor(const QColor &color);
        void setRawDicomVolume(const RawDicomVolumePtr &rawDicomVolume);
        void setVolumeInterpolator(const VolumeInterpolatorPtr &volumeInterpolator);
        void setVolumeSegmentator(const QSharedPointer<VolumeSegmentator> &volumeSegmentator);
        void setMeshSimplificator(const MeshSimplificatorPtr &meshSimplificator);
        void setBaseMesh(const Sokar3D::MeshPtr &baseMesh);
        void setUseEmptyEnv(bool useEmptyEnv);
        void setUseRegionGrowth(bool useRegionGrowth);
        void setGrowthStartPoint(const glm::i32vec3 &growthStartPoint);
        void setIsoRange(const Range<float> &isoRange);
        void setCubesPerMM(float cubes_per_mm);
        void setGradientVolume(const GradientVolumePtr &gradientVolume);

        QFuture<SegmentationResultCPtr> executePipeline();

    signals:
        void updateProgress(QString title, float progress);
    };
}

