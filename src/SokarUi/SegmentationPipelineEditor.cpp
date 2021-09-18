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
#include "SokarAlg/EdgeCollapse.hpp"
#include "Sokar3D/TriangleListMesh.hpp"
#include "Sokar3D/IndexedMesh.hpp"
#include "Sokar3D/CubedIndexedMesh.hpp"

using namespace SokarUi;

enum Methods {
    Nearest = 0,
    Linear = 1,
    Poly1 = 2,
    Poly2 = 3,
    Cubic = 4,
    CalumRom = 5,

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

SokarAlg::SegmentationPipelinePtr SegmentationPipelineEditor::makePipeline() const {

    auto pipeline = SokarAlg::SegmentationPipeline::New();

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

            case Methods::Cubic:
                volumeInterpolator = QSharedPointer<SokarAlg::CubicVolumeInterpolator>::create();
                break;

            case Methods::CalumRom:
                volumeInterpolator = QSharedPointer<SokarAlg::CubicVolumeInterpolator>::create(true);
                break;
        }

        pipeline->setVolumeInterpolator(volumeInterpolator);

        pipeline->getDicomVolume()->setCubesPerMM(float(ui->interpolationWokselSize->value()));
        pipeline->setUseInterpolationCache(ui->cacheInterpolation->isChecked());
    }

    //endregion

    //region segmentation

    QSharedPointer<SokarAlg::VolumeSegmentator> volumeSegmentator = nullptr;

    switch (ui->segmentationAlgorithm->currentIndex()) {

        case Methods::MarchingCubes:
            volumeSegmentator = SokarAlg::MarchingCubes::New();
            break;

        case Methods::MarchingTetra:
            volumeSegmentator = SokarAlg::MarchingTetrahedrons::New();
            break;
    }

    pipeline->setIsoRange({
                                  static_cast<float>(ui->segmentationTresholdUp->value()),
                                  static_cast<float>(ui->segmentationTresholdDown->value()),
                          });
    pipeline->setVolumeSegmentator(volumeSegmentator);

    pipeline->setUseRegionGrowth(ui->regionGrowthBox->isChecked());
    auto growStartPoint = ui->regionGrowthVec->getValue() / float(ui->interpolationWokselSize->value());
    pipeline->setGrowthStartPoint(growStartPoint);

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
    }

    //endregion

    //region simplification

    if (ui->simplificationBox->isChecked()) {
        switch (ui->simplificationAlgorithm->currentIndex()) {
            case 0: {
                auto vertexClustering = SokarAlg::VertexClustering::New();
                vertexClustering->setClusterSize(ui->vertexClusteringSize->getValue());
                vertexClustering->setClusterOffset(ui->vertexClusteringOffset->getValue());
                pipeline->setMeshSimplificator(vertexClustering);
                break;
            }
            case 1: {
                auto edgeCollapse = SokarAlg::EdgeCollapse::New();

                switch (ui->edgeCollapseDirection->currentIndex()) {
                    case 0: {
                        edgeCollapse->setDirection(SokarAlg::EdgeCollapse::DecimationDirection::Any);
                        break;
                    }
                    case 1: {
                        edgeCollapse->setDirection(SokarAlg::EdgeCollapse::DecimationDirection::Inward);
                        break;
                    }
                    case 2: {
                        edgeCollapse->setDirection(SokarAlg::EdgeCollapse::DecimationDirection::Outward);
                        break;
                    }
                }

                edgeCollapse->setMaximumError(ui->edgeCollapseError->value() / 100);
                edgeCollapse->setVertexReduction(ui->edgeCollapseVertReduction->value() / 100);
                pipeline->setMeshSimplificator(edgeCollapse);
                break;
            }
        }
    }

    //endregion


    //region design

    pipeline->setColor(meshColor);
    pipeline->setUseEmptyEnv(ui->emptyEnv->isChecked());

    //endregion

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
