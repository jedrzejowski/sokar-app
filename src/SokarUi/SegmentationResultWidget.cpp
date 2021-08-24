//
// Created by adam on 07.06.2021.
//

#include <QtConcurrent/QtConcurrent>
#include "SegmentationResultWidget.hpp"
#include "SokarLib/WavefrontObjBuilder.hpp"
#include "Sokar3D/TriangleListMesh.hpp"
#include "ui_SegmentationResultWidget.h"

using namespace SokarUi;

SegmentationResultWidget::SegmentationResultWidget(
        const SokarAlg::SegmentationResultCPtr &result,
        QWidget *parent
) : QFrame(parent),
    result(result),
    ui(new Ui::SegmentationResultWidget) {

    ui->setupUi(this);

    QObject::connect(ui->deleteButton, &QPushButton::clicked, [this]() { emit deleteResult(); });
    QObject::connect(ui->saveButton, &QPushButton::clicked, this, &SegmentationResultWidget::saveToWavefrontObjFile);
    QObject::connect(ui->hideButton, &QPushButton::clicked, [this](bool checked) { emit toggleMesh(checked); });

    auto pal = QPalette();
    pal.setColor(QPalette::Window, result->meshColor);
    ui->colorPreview->setAutoFillBackground(true);
    ui->colorPreview->setPalette(pal);

    ui->colorName->setText(result->meshColor.name());

    ui->segmentationResult->setText(result->segmentation.description);
    ui->interpolationResult->setText(result->interpolation.description);
    ui->cacheResult->setText(result->interpolationCache.description);
    ui->regionGrowthResult->setText(result->regionGrowth.description);
    ui->meshSimplificationLabel->setText(result->simplification.description);
    ui->sumLabel->setText(result->summary.description);
}

SokarUi::SegmentationResultWidget::~SegmentationResultWidget() {

    delete ui;
}

void SegmentationResultWidget::saveToWavefrontObjFile() {

    auto fileName = QFileDialog::getSaveFileName(
            this,
            tr("Save Wavefront OBJ"), "",
            tr("Wavefront (*.obj);;All Files (*)"));


    QtConcurrent::run(
            [
                    this,
                    fileName,
                    mesh = result->summary.mesh
            ]() {
                QFile file(fileName);

                if (!file.open(QIODevice::WriteOnly)) {
                    QMessageBox::information(
                            this, tr("Unable to open file"),
                            file.errorString());
                    return;
                }

                auto wavefront = SokarLib::WavefrontObjBuilder();
                mesh->dump2wavefront(wavefront);
                wavefront.save2file(file);
            });
}
