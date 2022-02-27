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

    ui->iso_range_label->setText(QString("[%1; %2]").arg(result->iso_range.from).arg(result->iso_range.to));
    ui->woksel_size_label->setText(
            QString("%1[mm] x %2[mm] x %3[mm]")
                    .arg(result->volume_interpolation.woksel_size.x)
                    .arg(result->volume_interpolation.woksel_size.y)
                    .arg(result->volume_interpolation.woksel_size.z)
    );
    ui->volume_interpolation_label->setText(result->volume_interpolation.description);
    ui->volume_interpolation_cache_label->setText(result->volume_interpolation_cache.description);
    ui->region_growth_label->setText(result->region_growth.description);
    ui->gradient_label->setText(result->gradient.description);
    ui->segmentation_label->setText(result->segmentation.description);
    ui->line_interpolation_label->setText(result->line_interpolation.description);
    ui->mesh_simplification_label->setText(result->mesh_simplification.description);
    ui->volume_env_label->setText(result->volume_env.description);
    ui->sumLabel->setText(result->summary.description);

//    ui->volume_interpolation_time->setText(result->volume_interpolation.description);
    ui->volume_interpolation_cache_time->setText(SokarAlg::timeRangeString(
            result->volume_interpolation_cache.timeStart, result->volume_interpolation_cache.timeEnd));
    ui->region_growth_time->setText(SokarAlg::timeRangeString(
            result->region_growth.timeStart, result->region_growth.timeEnd));
//    ui->gradient_time->setText(result->gradient.description);
    ui->segmentation_time->setText(SokarAlg::timeRangeString(
            result->segmentation.timeStart, result->segmentation.timeEnd));
//    ui->line_interpolation_time->setText(result->line_interpolation.description);
    ui->mesh_simplification_time->setText(SokarAlg::timeRangeString(
            result->mesh_simplification.timeStart, result->mesh_simplification.timeEnd));
//    ui->volume_env_time->setText(result->volume_env.description);

//    timeRangeString
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
