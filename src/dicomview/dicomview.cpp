#include "dicomview.h"
#include "ui_dicomview.h"

#include "scenes/sets/frameset.h"

using namespace Sokar;

DicomView::DicomView(DicomSceneSet *dicomSceneSet, QWidget *parent) :
		QWidget(parent),
		dicomSceneSet(dicomSceneSet),
		ui(new Ui::DicomView) {

	ui->setupUi(this);

	dicomSceneSet->setParent(this);

	connect(ui->toolbar, &DicomToolBar::stateToggleSignal, this, &DicomView::toolbarStateToggle);
	connect(ui->toolbar, &DicomToolBar::actionTriggerSignal, this, &DicomView::toolbarActionTrigger);
	connect(ui->frameChooser, &FrameChooser::setStep, this, &DicomView::setStep);
	connect(ui->movieBar, &MovieBar::setStep, this, &DicomView::setStep);
	connect(this, &DicomView::stepChanged, ui->frameChooser, &FrameChooser::stepChanged);
	connect(this, &DicomView::stepChanged, ui->movieBar, &MovieBar::stepChanged);

	ui->frameChooser->setSceneSet(dicomSceneSet);
	ui->movieBar->setSceneSet(dicomSceneSet);

	// Aktywacja pierwszego kroku
	{
		auto sequence = dicomSceneSet->getSceneSequence();
		if (sequence->size() == 1)
			setStep(sequence->step());
		else ui->movieBar->start();
	}
}

DicomView::~DicomView() {
	qDebug() << "~DicomView()";

	// Po to aby timer nie wywołał jakieś funkcji w czasie usuwania
	ui->movieBar->stop();

	delete ui;
	delete dicomSceneSet;
}

DicomScene *DicomView::getDicomScene() {
	return (DicomScene *) ui->graphicsView->scene();
}

void DicomView::setStep(const Step *step) {
	auto *scene = step->scene;

	scene->setSceneRect(0, 0,
						ui->graphicsView->width(),
						ui->graphicsView->height());

	ui->graphicsView->setScene(scene);

	scene->reposItems();
	scene->toolBarAdjust(ui->toolbar);
	scene->reloadPixmap();
	scene->updatePixmapTransformation();

	emit stepChanged(step);
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

			if (getDicomScene() == nullptr) break;

			DataSetViewer::openAsWindow(getDicomScene());

			return;

		case DicomToolBar::ClearPan:
		case DicomToolBar::Fit2Screen:
		case DicomToolBar::OriginalResolution:
		case DicomToolBar::RotateRight90:
		case DicomToolBar::RotateLeft90:
		case DicomToolBar::FlipHorizontal:
		case DicomToolBar::FlipVertical:
		case DicomToolBar::ClearRotate:
			if (getDicomScene() == nullptr) break;
			getDicomScene()->toolBarActionSlot(action);
			break;
	}
}

void DicomView::toolbarStateToggle(DicomToolBar::State state) {
}
