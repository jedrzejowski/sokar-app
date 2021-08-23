//
// Created by adam on 21.05.2021.
//

#pragma once

#include <QMainWindow>
#include "SokarUi.hpp"
#include <Sokar3D/VulkanRenderer.hpp>
#include <Sokar3D/VulkanWidget.hpp>
#include <SokarAlg/DicomVolume.hpp>

namespace Ui {
    class SegmentationWindow;
}

namespace SokarUi {

    class SegmentationWindow : public QMainWindow {
    Q_OBJECT
        QProgressDialog *progressDialog;
        QSharedPointer<const SokarAlg::RawDicomVolume> rawDicomVolume;
        Ui::SegmentationWindow *ui;
        Sokar3D::VulkanWidget *vulkanWidget;
        Sokar3D::VulkanRenderer *vulkanRenderer;
        SegmentationPipelineEditor *pipelineEditor;

    public:
        explicit SegmentationWindow(QWidget *parent = nullptr);

        ~SegmentationWindow() override;

        void setRawDicomVolume(const QSharedPointer<const SokarAlg::RawDicomVolume> &rawDicomVolume);

    private slots:
        void startSegmentation(bool append = false);
        void endSegmentation(QSharedPointer<const SokarAlg::SegmentationResult> mesh);
    };
}



