//
// Created by adam on 20.05.2021.
//

#pragma once

#include "SokarUi.hpp"

#include <QWidget>
#include <SokarAlg/DicomVolume.hpp>
#include <SokarAlg/VolumeSegmentator.hpp>

namespace Ui {
    class SokarSegmentationPipelineEditor;
}

namespace SokarUi {

    class SegmentationPipelineEditor : public QWidget {
    Q_OBJECT
        Ui::SokarSegmentationPipelineEditor *ui;
        QColor meshColor = QColor("#BF4024");
        void setupUi();

    public:
        explicit SegmentationPipelineEditor(QWidget *parent = nullptr);
        ~SegmentationPipelineEditor() override;

        //nie może być const, bo dialogi by nie działały
        [[nodiscard]]
        SokarAlg::SegmentationPipelinePtr makePipeline();

        void setMeshColor(const QColor &color);

    public slots:
        void randomizeMeshColor();

    private slots:
        void simplificationAlgorithmComboBoxIndexChanged(int i);
    };
}
