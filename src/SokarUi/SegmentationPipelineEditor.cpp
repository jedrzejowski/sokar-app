//
// Created by adam on 20.05.2021.
//

#include <SokarException.hpp>
#include <SokarAlg/VertexClustering.hpp>
#include "SegmentationPipelineEditor.hpp"
#include "ui_SegmentationPipelineEditor.h"
#include "SokarAlg/SegmentationPipeline.hpp"
#include "SokarAlg/MarchingCubes.hpp"
#include "SokarAlg/MarchingTetrahedrons.hpp"
#include "SokarAlg/BoundingMeshSimplification.hpp"
#include "SokarAlg/EdgeCollapseSimplification.hpp"
#include "SokarAlg/GradientVolume.hpp"
#include "SokarAlg/DebugVolume.hpp"
#include "SokarAlg/LineInterpolator.hpp"
#include "Sokar3D/TriangleListMesh.hpp"
#include "Sokar3D/IndexedMesh.hpp"
#include "Sokar3D/CubedIndexedMesh.hpp"

using namespace SokarUi;

enum Methods {
    Nearest = 0,
    Linear = 1,
    Poly1 = 2,
    Poly2 = 3,
    Cubic1 = 4,
    Cubic1CalumRom = 5,
    Cubic2 = 6,

    MarchingCubes = 0,
    MarchingTetra = 1,
};


SegmentationPipelineEditor::SegmentationPipelineEditor(QWidget *parent)
        : QWidget(parent),
          ui(new Ui::SokarSegmentationPipelineEditor) {

    setupUi();
}

SegmentationPipelineEditor::~SegmentationPipelineEditor() {

    delete ui;
}

void SegmentationPipelineEditor::setupUi() {

    ui->setupUi(this);

    setMeshColor(meshColor);
    QObject::connect(
            ui->pickColorButton,
            &QPushButton::clicked,
            [this]() {
                setMeshColor(QColorDialog::getColor(meshColor, this, "Wybierz kolor"));
            });

    QObject::connect(
            ui->randomColorButton, &QPushButton::clicked,
            this, &SegmentationPipelineEditor::randomizeMeshColor);

    simplificationAlgorithmComboBoxIndexChanged(ui->simplificationAlgorithm->currentIndex());
    QObject::connect(
            ui->simplificationAlgorithm, qOverload<int>(&QComboBox::currentIndexChanged),
            this, &SegmentationPipelineEditor::simplificationAlgorithmComboBoxIndexChanged);

    ui->vertexClusteringSize->setValue({10.f, 10.f, 10.f});
    ui->vertexClusteringSize->setMinimum(0.01f);
    ui->vertexClusteringSize->setMaximum(65536.f);

    ui->gradient_box->setVisible(false);

}

void SegmentationPipelineEditor::simplificationAlgorithmComboBoxIndexChanged(int i) {

    ui->vertexClustering->setHidden(true);
    ui->edgeCollapse->setHidden(true);

    switch (i) {
        case 1:
            ui->vertexClustering->setHidden(false);
            break;
        case 2:
            ui->edgeCollapse->setHidden(false);
            break;

    }
}

SokarAlg::SegmentationPipelinePtr SegmentationPipelineEditor::makePipeline(
        const SokarAlg::RawDicomVolumePtr &raw_dicom_volume
) {

    auto pipeline = SokarAlg::SegmentationPipeline::New();
    pipeline->setRawDicomVolume(raw_dicom_volume);

    //region
    switch (ui->debug_replace_volume->currentIndex()) {

        case 1:
            pipeline->setRawDicomVolume(SokarAlg::DebugSphereVolume::create(raw_dicom_volume));
            break;

        case 2:
            pipeline->setRawDicomVolume(SokarAlg::DebugCubeVolume::create(raw_dicom_volume));
            break;

    }
    //endregion

    //region interpolation

    if (ui->interpolationBox->isChecked()) {
        SokarAlg::VolumeInterpolatorPtr volumeInterpolator = nullptr;

        switch (ui->interpolationCombo->currentIndex()) {

            case Methods::Nearest:
                volumeInterpolator = QSharedPointer<SokarAlg::NearestVolumeInterpolator>::create();
                break;

            case Methods::Linear:
                volumeInterpolator = QSharedPointer<SokarAlg::LinearVolumeInterpolator>::create();
                break;

            case Methods::Poly1:
                volumeInterpolator = QSharedPointer<SokarAlg::PolynomialVolumeInterpolator1>::create();
                break;

            case Methods::Poly2:
                volumeInterpolator = QSharedPointer<SokarAlg::PolynomialVolumeInterpolator2>::create();
                break;

            case Methods::Cubic1:
                volumeInterpolator = QSharedPointer<SokarAlg::CubicVolumeInterpolator1>::create();
                break;

            case Methods::Cubic1CalumRom:
                volumeInterpolator = QSharedPointer<SokarAlg::CubicVolumeInterpolator1>::create(true);
                break;

            case Methods::Cubic2:
                volumeInterpolator = QSharedPointer<SokarAlg::CubicVolumeInterpolator2>::create();
                break;
        }

        pipeline->setVolumeInterpolator(volumeInterpolator);

        pipeline->setTargetWokselSize(float(ui->interpolationWokselSize->value()));
        pipeline->setUseCache(ui->cache_box->isChecked());
        pipeline->setCacheThreads(ui->cahce_thread_count->value());
    }

    //endregion

    //region region_growth

    if (ui->region_growth_box->isChecked()) {

        pipeline->setUseRegionGrowth(true);
        pipeline->setGrowthStartPoint(ui->region_growth_vec->getValue());
    }

    //endregion

    //region gradient

    if (ui->gradient_box->isChecked()) {
        auto gradient_volume = SokarAlg::GradientVolume::New();

        if (ui->gradient_check_x->isChecked())
            gradient_volume->addDirection(SokarAlg::GradientVolume::X);
        if (ui->gradient_check_y->isChecked())
            gradient_volume->addDirection(SokarAlg::GradientVolume::Y);
        if (ui->gradient_check_z->isChecked())
            gradient_volume->addDirection(SokarAlg::GradientVolume::Z);
        if (ui->gradient_check_xy->isChecked())
            gradient_volume->addDirection(SokarAlg::GradientVolume::XY);
        if (ui->gradient_check_xz->isChecked())
            gradient_volume->addDirection(SokarAlg::GradientVolume::XZ);
        if (ui->gradient_check_yz->isChecked())
            gradient_volume->addDirection(SokarAlg::GradientVolume::YZ);
        if (ui->gradient_check_xyz->isChecked())
            gradient_volume->addDirection(SokarAlg::GradientVolume::XYZ);

        switch (ui->gradient_merge_combo->currentIndex()) {
            case 0:
                gradient_volume->setMergeStrategy(SokarAlg::GradientVolume::Sum);
                break;
            case 1:
                gradient_volume->setMergeStrategy(SokarAlg::GradientVolume::Highest);
                break;
            case 2:
                gradient_volume->setMergeStrategy(SokarAlg::GradientVolume::Average);
                break;
            case 3:
                gradient_volume->setMergeStrategy(SokarAlg::GradientVolume::RootMeanSquare);
                break;
            case 4:
                gradient_volume->setMergeStrategy(SokarAlg::GradientVolume::Median);
                break;

            default:
                Q_ASSERT(false);
        }

        pipeline->setGradientVolume(gradient_volume);
    }

    //endregion

    //region segmentation

    SokarAlg::VolumeSegmentatorPtr volumeSegmentator = nullptr;

    switch (ui->segmentationAlgorithm->currentIndex()) {

        case Methods::MarchingCubes:
            volumeSegmentator = SokarAlg::MarchingCubes::New();
            break;

        case Methods::MarchingTetra:
            volumeSegmentator = SokarAlg::MarchingTetrahedrons::New();
            break;

        default:
            Q_ASSERT(false);
    }

    pipeline->setIsoRange({
                                  static_cast<float>(ui->segmentation_treshold_down->value()),
                                  static_cast<float>(ui->segmentation_treshold_up->value()),
                          });
    pipeline->setVolumeSegmentator(volumeSegmentator);

    switch (ui->meshTypeCombo->currentIndex()) {
        case 0: {
            pipeline->setBaseMesh(Sokar3D::TriangleListMesh::New());
            break;
        }
        case 1: {
            pipeline->setBaseMesh(Sokar3D::IndexedMesh::New());
            break;
        }
        case 2: {
            pipeline->setBaseMesh(Sokar3D::CubedIndexedMesh::New());
            break;
        }
        default:
            Q_ASSERT(false);
    }

    //endregion

    //region line interpolation

    SokarAlg::LineInterpolatorPtr interpolator;

    switch (ui->line_interpolation_combo->currentIndex()) {
        case 0: {
            interpolator = SokarAlg::LinearLineInterpolator::New();

            pipeline->setLineInterpolator(interpolator);
            break;
        }
        case 1: {
            interpolator = SokarAlg::HalfLineInterpolator::New();

            pipeline->setLineInterpolator(interpolator);
            break;
        }
        case 2: {
            interpolator = SokarAlg::PolynomialLineInterpolator::New();

            pipeline->setLineInterpolator(interpolator);
            break;
        }
        case 3: {
            interpolator = SokarAlg::SplineLineInterpolator::New();

            auto extend_point = ui->line_interpolation_extend_point->value();

            if (extend_point < 1) {
                QMessageBox::warning(this, "Błąd", "Ilośc dodatkowych punktów musi być większa niż zero");
                return nullptr;
            }

            pipeline->setLineInterpolator(interpolator);
            break;
        }
        default:
            Q_ASSERT(false);
    }

    interpolator->setExtendPointCount(ui->line_interpolation_extend_point->value());
    interpolator->setMaxIter(ui->line_interpolation_max_iter->value());
    switch (ui->line_interpolation_iter_method->currentIndex()) {
        case 0:
            interpolator->setMethod(SokarAlg::Bisection);
            break;
        case 1:
            interpolator->setMethod(SokarAlg::FalsePosition);
            break;
    }


    //endregion

    //region simplification

    if (ui->simplification_box->isChecked()) {
        switch (ui->simplificationAlgorithm->currentIndex()) {
            case 0: {
                auto vertexClustering = SokarAlg::VertexClustering::New();
                vertexClustering->setClusterSize(ui->vertexClusteringSize->getValue());
                vertexClustering->setClusterOffset(ui->vertexClusteringOffset->getValue());
                pipeline->setMeshSimplificator(vertexClustering);
                break;
            }
            case 1: {
                auto edge_collapse = SokarAlg::EdgeCollapseSimplification::New();
                edge_collapse->setVertexReduction(0.01);
                pipeline->setMeshSimplificator(edge_collapse);
                break;
            }
            case 2: {
                auto bouding_mesh = SokarAlg::BoundingMeshSimplification::New();

                switch (ui->edgeCollapseDirection->currentIndex()) {
                    case 0: {
                        bouding_mesh->setDirection(SokarAlg::BoundingMeshSimplification::DecimationDirection::Any);
                        break;
                    }
                    case 1: {
                        bouding_mesh->setDirection(SokarAlg::BoundingMeshSimplification::DecimationDirection::Inward);
                        break;
                    }
                    case 2: {
                        bouding_mesh->setDirection(SokarAlg::BoundingMeshSimplification::DecimationDirection::Outward);
                        break;
                    }
                }

                bouding_mesh->setMaximumError(ui->edgeCollapseError->value() / 100);
                bouding_mesh->setVertexReduction(ui->edgeCollapseVertReduction->value() / 100);
                pipeline->setMeshSimplificator(bouding_mesh);
                break;
            }
            default:
                Q_ASSERT(false);
        }
    }

    //endregion


    //region empty_env_type

    switch (ui->emptyEnvCombo->currentIndex()) {
        case 0:
            pipeline->setEmptyEnvType(SokarAlg::VolumeEnv::No);
            break;
        case 1:
            pipeline->setEmptyEnvType(SokarAlg::VolumeEnv::Yes);
            break;
        case 2:
            pipeline->setEmptyEnvType(SokarAlg::VolumeEnv::CircleX);
            break;
        case 3:
            pipeline->setEmptyEnvType(SokarAlg::VolumeEnv::CircleY);
            break;
        case 4:
            pipeline->setEmptyEnvType(SokarAlg::VolumeEnv::CircleZ);
            break;
    }

    //endregion

    pipeline->setColor(meshColor);

    return pipeline;
}

void SegmentationPipelineEditor::randomizeMeshColor() {

    static auto generator = QRandomGenerator();
    auto color = QColor();
    color.setHslF(
            generator.generateDouble(),
            0.5,
            0.5
    );
    setMeshColor(color);
}

void SegmentationPipelineEditor::setMeshColor(const QColor &color) {

    meshColor = color;
    ui->pickColorButton->setText(meshColor.name());
}
