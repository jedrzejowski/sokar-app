
#include <QtCore>

#include "dicomview.h"
#include "ui_dicomview.h"


using namespace Sokar;

DicomView::DicomView(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::DicomView) {

	ui->setupUi(this);
	ui->frameChooser->hide();

	connect(ui->frameChooser, &FrameChooser::selectSceneSignal, this, &DicomView::activateScene);

}

DicomView::~DicomView() {
	delete ui;
}

DicomScene* DicomView::currentDicomScene() {
	return (DicomScene *) ui->graphicsView->scene();
}

void DicomView::addDicomImage(const gdcm::ImageReader *reader) {

	auto dicomSceneSet = new DicomSceneSet(reader, this);

	sceneSets << dicomSceneSet;

	ui->frameChooser->addSceneSet(dicomSceneSet);

	if (sceneSets.size() > 1 || dicomSceneSet->getVector().size() > 1)
		ui->frameChooser->show();
}

void DicomView::activateScene(DicomScene *scene) {

	scene->setSceneRect(0, 0,
						ui->graphicsView->width(),
						ui->graphicsView->height());

	ui->graphicsView->setScene(scene);

	scene->reposItems();
	scene->reposItems();
}
