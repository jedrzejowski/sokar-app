//
// Created by adam on 21.05.2021.
//

#include <QtConcurrent/QtConcurrentRun>
#include "SegmentationWindow.hpp"
#include "ui_SegmentationWindow.h"
#include "SokarAlg/SegmentationPipeline.hpp"
#include "Sokar3D/CenterCamera.hpp"
#include "Sokar3D/MeshPipeline.hpp"
#include "SegmentationResultWidget.hpp"

using namespace SokarUi;

SegmentationWindow::SegmentationWindow(QWidget *parent)
		: QMainWindow(parent),
		  ui(new Ui::SegmentationWindow) {
	ui->setupUi(this);

	// usuwa okno od razu po zamknięciu
	// https://stackoverflow.com/questions/15814626/how-to-show-a-window-in-qt-and-deleting-it-as-soon-as-its-closed
	setAttribute(Qt::WA_DeleteOnClose);

	auto ret = Sokar3D::VulkanWidget::New<Sokar3D::VulkanRenderer>();
	vulkanWidget = ret.vulkanWidget;
	vulkanRenderer = ret.vulkanRenderer;
	ui->mainSplitter->insertWidget(0, ret.widget);
	ui->vulkanPlacehodler->hide();

	QObject::connect(ui->execSegementation, &QPushButton::clicked, [this]() { startSegmentation(); });
	QObject::connect(ui->execSegementation2, &QPushButton::clicked, [this]() { startSegmentation(true); });

	pipelineEditor = ui->pipelineEditor;
}

SegmentationWindow::~SegmentationWindow() {
	delete ui;
}

void SegmentationWindow::setRawDicomVolume(const QSharedPointer<const SokarAlg::RawDicomVolume> &newRawDicomVolume) {
	rawDicomVolume = newRawDicomVolume;
}

void SegmentationWindow::startSegmentation(bool append) {

	progressDialog = new QProgressDialog(this);
	progressDialog->setAttribute(Qt::WA_DeleteOnClose);
	progressDialog->setWindowTitle("Segmentowanie");
	progressDialog->setModal(true);
	progressDialog->setCancelButton(nullptr);
	progressDialog->show();


	auto segmentationPipeline = pipelineEditor->makePipeline();
	segmentationPipeline->setRawDicomVolume(rawDicomVolume);

	auto future = segmentationPipeline->executePipeline();

	auto watcher = new QFutureWatcher<QSharedPointer<const SokarAlg::SegmentationResult>>();

	QObject::connect(watcher, &QFutureWatcherBase::finished, [=, this]() {
		emit endSegmentation(future.result());
		watcher->deleteLater();
	});

	watcher->setFuture(future);

	pipelineEditor->randomizeMeshColor();
}

void SegmentationWindow::endSegmentation(QSharedPointer<const SokarAlg::SegmentationResult> result) {

	progressDialog->close();
	progressDialog = nullptr;

	auto camera = new Sokar3D::CenterCamera(
			result->proposeCameraCenter,
			result->proposeCameraDistance
	);
	vulkanRenderer->setCamera(camera);

	auto graphicPipeline = new Sokar3D::MeshPipeline(result->mesh);

	Sokar3D::SolidMaterial material{};
	material.shininess = 64.0f;
	material.specular = glm::vec3(0.5f, 0.5f, 0.5f);
	material.color = glm::vec3(
			result->meshColor.redF(),
			result->meshColor.greenF(),
			result->meshColor.blueF()
	);
	graphicPipeline->setMeshMaterial(material);

	auto resultWidget = new SegmentationResultWidget(result);
	resultWidget->moveToThread(this->thread());

	// wsadż na przedostatnią pozycję
	ui->resultLayout->insertWidget(ui->resultLayout->count() - 1, resultWidget);

	vulkanRenderer->addPipeline(graphicPipeline);

	QObject::connect(resultWidget, &SegmentationResultWidget::deleteResult, [this, graphicPipeline](){
		vulkanRenderer->removePipeline(graphicPipeline);
	});
}
