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

        RawDicomVolumePtr rawDicomVolume = nullptr;
        DicomVolumePtr dicomVolume = nullptr;
        Sokar3D::MeshPtr baseMesh = nullptr;
        VolumeInterpolatorPtr volumeInterpolator = nullptr;
        VolumeSegmentatorPtr volumeSegmentator = nullptr;
        MeshSimplificatorPtr meshSimplificator = nullptr;

        SegmentationPipeline();

    public:

        static SegmentationPipelinePtr New();

        bool isUseInterpolationCache() const;
        void setUseInterpolationCache(bool useCache);
        const QColor &getColor() const;
        void setColor(const QColor &color);
        const RawDicomVolumePtr &getRawDicomVolume() const;
        void setRawDicomVolume(const RawDicomVolumePtr &rawDicomVolume);
        const QSharedPointer<DicomVolume> &getDicomVolume() const;
        void setDicomVolume(const QSharedPointer<DicomVolume> &dicomVolume);
        const VolumeInterpolatorPtr &getVolumeInterpolator() const;
        void setVolumeInterpolator(const VolumeInterpolatorPtr &volumeInterpolator);
        const VolumeSegmentatorPtr &getVolumeSegmentator() const;
        void setVolumeSegmentator(const QSharedPointer<VolumeSegmentator> &volumeSegmentator);
        const MeshSimplificatorPtr &getMeshSimplificator() const;
        void setMeshSimplificator(const MeshSimplificatorPtr &meshSimplificator);
        const Sokar3D::MeshPtr &getBaseMesh() const;
        void setBaseMesh(const Sokar3D::MeshPtr &baseMesh);
        bool isUseEmptyEnv() const;
        void setUseEmptyEnv(bool useEmptyEnv);
        bool isUseRegionGrowth() const;
        void setUseRegionGrowth(bool useRegionGrowth);
        const glm::i32vec3 &getGrowthStartPoint() const;
        void setGrowthStartPoint(const glm::i32vec3 &growthStartPoint);
        const Range<float> &getIsoRange() const;
        void setIsoRange(const Range<float> &isoRange);

        QFuture<SegmentationResultCPtr> executePipeline();

    signals:
        void updateProgress(QString title, float progress);
    };
}

