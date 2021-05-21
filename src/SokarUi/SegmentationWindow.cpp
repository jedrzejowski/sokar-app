//
// Created by adam on 21.05.2021.
//

#include "SegmentationWindow.hpp"
#include "ui_SegmentationWindow.h"
#include "SegmentationPipelineEditor.hpp"

using namespace SokarUi;

SegmentationWindow::SegmentationWindow(QWidget *parent)
		: QMainWindow(parent), ui(new Ui::SegmentationWindow) {
	ui->setupUi(this);

	// usuwa okno od razu po zamknięciu
	// https://stackoverflow.com/questions/15814626/how-to-show-a-window-in-qt-and-deleting-it-as-soon-as-its-closed
	setAttribute(Qt::WA_DeleteOnClose);

	auto ret = Sokar3D::VulkanWidget::New<Sokar3D::VulkanRenderer>();

	vulkanWidget = ret.window;
	vulkanRenderer = ret.renderer;

	ui->mainSplitter->addWidget(ret.wrapper);

	ui->mainSplitter->addWidget(pipelineEditor = new SegmentationPipelineEditor());
}

SegmentationWindow::~SegmentationWindow() {
	delete ui;
}

void SegmentationWindow::setRawDicomVolume(const QSharedPointer<const SokarAlg::RawDicomVolume> &newRawDicomVolume) {
	rawDicomVolume = newRawDicomVolume;
}
