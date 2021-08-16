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

SokarScene::DicomScene *DicomView::getDicomScene() {
	return (SokarScene::DicomScene *) ui->graphicsView->scene();
}

void DicomView::setStep(const Step *step) {
	auto *scene = step->scene;

	scene->setSceneRect(0, 0,
						ui->graphicsView->width(),
						ui->graphicsView->height());

	ui->graphicsView->setScene(scene);

	scene->prepare();
	scene->attached();

	emit stepChanged(step);
}

DicomToolBar *DicomView::getToolBar() {
	return ui->toolbar;
}

FrameChooser *DicomView::getFrameChooser() {
	return ui->frameChooser;
}

void DicomView::toolbarActionTrigger(DicomToolBar::Action action, bool state) {
	switch (action) {
		case DicomToolBar::OpenDataSet:

			if (getDicomScene() == nullptr) break;

			DataSetViewer::openAsWindow(getDicomScene());

			return;

		default:
			if (getDicomScene() == nullptr) break;
			getDicomScene()->toolBarActionSlot(action, state);
			break;
	}
}

void DicomView::toolbarStateToggle(DicomToolBar::State state) {
}
