
#include <QtCore>

#include "dicomview.h"

using namespace Sokar;

DicomView::DicomView(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::DicomView) {

	ui->setupUi(this);
	ui->frameChooser->hide();

}

DicomView::~DicomView() {
	delete ui;
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
	scene->reposItems();

	ui->graphicsView->setScene(scene);
}
