#include "dicomview.h"
#include "ui_dicomview.h"

using namespace Sokar;

DicomView::DicomView(const gdcm::ImageReader *reader, QWidget *parent) :
		QWidget(parent),
		ui(new Ui::DicomView) {


	ui->setupUi(this);

	connect(ui->frameChooser, &FrameChooser::selectSceneSignal, this, &DicomView::activateScene);
	connect(ui->toolbar, &DicomToolBar::stateToggleSignal, this, &DicomView::toolbarStateToggle);
	connect(ui->toolbar, &DicomToolBar::actionTriggerSignal, this, &DicomView::toolbarActionTrigger);

	dicomSceneSet = new DicomSceneSet(reader, this);

	ui->frameChooser->setSceneSet(dicomSceneSet);

	if (dicomSceneSet->getScenesVector().size() == 1)
		ui->frameChooser->hide();
}

DicomView::~DicomView() {
	qDebug() << "~DicomView()";
	delete ui;
	delete dicomSceneSet;
}

DicomScene *DicomView::currentDicomScene() {
	return (DicomScene *) ui->graphicsView->scene();
}

void DicomView::activateScene(DicomScene *scene) {

	scene->setSceneRect(0, 0,
						ui->graphicsView->width(),
						ui->graphicsView->height());

	ui->graphicsView->setScene(scene);

	scene->reposItems();
	scene->toolBarAdjust(ui->toolbar);
}

DicomToolBar &DicomView::getToolBar() {
	return *(ui->toolbar);
}

FrameChooser &DicomView::getFrameChooser() {
	return *(ui->frameChooser);
}

void DicomView::toolbarActionTrigger(DicomToolBar::Action action) {
	switch (action) {
		case DicomToolBar::OpenDataSet:

			if (currentDicomScene() == nullptr) break;

			DataSetViewer::openAsWindow(currentDicomScene()->getDicomSceneSet());

			return;

		case DicomToolBar::ClearPan:
		case DicomToolBar::Fit2Screen:
		case DicomToolBar::OriginalResolution:
		case DicomToolBar::RotateRight90:
		case DicomToolBar::RotateLeft90:
		case DicomToolBar::FlipHorizontal:
		case DicomToolBar::FlipVertical:
		case DicomToolBar::ClearRotate:
			if (currentDicomScene() == nullptr) break;
			currentDicomScene()->toolBarActionSlot(action);
			break;
	}
}

void DicomView::toolbarStateToggle(DicomToolBar::State state) {
}
