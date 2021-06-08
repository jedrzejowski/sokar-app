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

	auto progressDialog = new QProgressDialog(this);
	progressDialog->setAttribute(Qt::WA_DeleteOnClose);
	progressDialog->setWindowTitle("Segmentowanie");
	progressDialog->setModal(true);
	progressDialog->setCancelButton(nullptr);
	progressDialog->show();

	QtConcurrent::run([this, progressDialog]() {

		auto segmentationPipeline = pipelineEditor->makePipeline();
		segmentationPipeline->setRawDicomVolume(rawDicomVolume);

		auto future = segmentationPipeline->executePipeline();
		future.waitForFinished();

		auto result = future.result();

		emit endSegmentation(result);

		progressDialog->close();
	});

//	pipeline = QSharedPointer<SokarAlg::SegmentationPipeline>::create();
//	pipeline->rawDicomVolume = rawDicomVolume;
//
//	auto future = pipeline->executePipeline();
//
//	auto watcher = new QFutureWatcher<SokarAlg::SegmentationResult>();
//	qDebug() << watcher;
//
//	QObject::connect(watcher, &QFutureWatcherBase::finished, [watcher, this]() {
//		qDebug() << "here";
//		qDebug() << watcher;
//		auto result = watcher->result();
//
//		auto camera = new Sokar3D::CenterCamera(
//				result.proposeCameraCenter,
//				result.proposeCameraDistance
//		);
//		vulkanRenderer->setCamera(camera);
//
//		auto pipeline = new Sokar3D::MeshPipeline(result.mesh);
//		vulkanRenderer->addPipelineWrapper(pipeline);
//	});
//
//	watcher->setFuture(future);
}

void SegmentationWindow::endSegmentation(QSharedPointer<const SokarAlg::SegmentationResult> result) {

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

//	auto resultWidget = new SegmentationResultWidget(result);
//
//	ui->resultLayout->addWidget(resultWidget);

	vulkanRenderer->addPipelineWrapper(graphicPipeline);
}
