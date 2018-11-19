
#include <QtCore>

#include "qdicomview.h"
#include "ui_qdicomview.h"

using namespace Sokar;

QDicomView::QDicomView(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::QDicomView) {
	ui->setupUi(this);

	sceneParams = new SceneParams();

//	setMouseTracking(true);
}

QDicomView::~QDicomView() {
	delete ui;
	delete sceneParams;
}

void QDicomView::resizeEvent(QResizeEvent *event) {
	QWidget::resizeEvent(event);

	if (currentDicomScene() != nullptr) {
		currentDicomScene()->setSceneRect(0, 0,
										  ui->graphicsView->width(),
										  ui->graphicsView->height());
		currentDicomScene()->reposItems();
	}
}

void QDicomView::addDicomImage(const gdcm::ImageReader &reader) {

	auto dicomScene = Sokar::DicomScene::createForImg(reader, sceneParams);
	scenes.push_back(dicomScene);

	activateScene(dicomScene);

}

DicomScene *QDicomView::currentDicomScene() {
	return (DicomScene *) ui->graphicsView->scene();
}

void QDicomView::activateScene(DicomScene *scene) {
	scene->setSceneRect(0, 0,
						ui->graphicsView->width(),
						ui->graphicsView->height());
	scene->reposItems();

	ui->graphicsView->setScene(scene);

	auto &file = scene->getGdcmFile();
	ui->datasetView->setGdcmFile(file);
}
