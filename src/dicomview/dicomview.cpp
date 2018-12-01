

#include "dicomview.h"
#include "ui_dicomview.h"


using namespace Sokar;

DicomView::DicomView(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::DicomView) {

	ui->setupUi(this);
	ui->frameChooser->hide();

	setAcceptDrops(true);

	connect(ui->frameChooser, &FrameChooser::selectSceneSignal, this, &DicomView::activateScene);
	connect(ui->toolbar, &DicomToolBar::stateToggleSignal, this, &DicomView::toolbarStateToggle);
	connect(ui->toolbar, &DicomToolBar::actionTriggerSignal, this, &DicomView::toolbarActionTrigger);
}

DicomView::~DicomView() {
	delete ui;
}

DicomScene *DicomView::currentDicomScene() {
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

void DicomView::dropEvent(QDropEvent *event) {
	const auto *mimeData = event->mimeData();

	if (mimeData->hasUrls()) {

		QStringList pathList;
		QList<QUrl> urlList = mimeData->urls();

		for (auto &path : mimeData->urls()) {

			auto *ir = new gdcm::ImageReader;

			ir->SetFileName(path.toLocalFile().toStdString().c_str());

			if (!ir->Read()) {
				QMessageBox::critical(this, "Error", "An error has occured !");
				delete ir;
				continue;
			}

			addDicomImage(ir);
		}

		event->acceptProposedAction();
	}
}

void DicomView::dragEnterEvent(QDragEnterEvent *event) {
	event->acceptProposedAction();
}

void DicomView::dragMoveEvent(QDragMoveEvent *event) {
	event->acceptProposedAction();
}

void DicomView::dragLeaveEvent(QDragLeaveEvent *event) {
	event->accept();
}
