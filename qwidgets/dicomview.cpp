
#include <QtCore>

#include "dicomview.h"

using namespace Sokar;

DicomView::DicomView(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::DicomView) {
	ui->setupUi(this);

}

DicomView::~DicomView() {
	delete ui;
}

void DicomView::resizeEvent(QResizeEvent *event) {
	QWidget::resizeEvent(event);

	if (currentDicomScene() != nullptr) {
		currentDicomScene()->setSceneRect(0, 0,
										  ui->graphicsView->width(),
										  ui->graphicsView->height());
		currentDicomScene()->reposItems();
	}
}

void DicomView::addDicomImage(const gdcm::ImageReader *reader) {

	auto dicomSceneSet = new Sokar::DicomSceneSet(reader);
	dicomSceneSet->setParent(this);
	files.push_back(dicomSceneSet);

	activateScene(dicomSceneSet->at(0));
}

void DicomView::activateScene(DicomScene *scene) {
	scene->setSceneRect(0, 0,
						ui->graphicsView->width(),
						ui->graphicsView->height());
	scene->reposItems();

	ui->graphicsView->setScene(scene);

	auto &file = scene->getGdcmFile();
	ui->datasetView->setGdcmFile(file);
}
