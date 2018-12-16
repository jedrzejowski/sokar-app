#include "sokar/settings.h"
#include "dicomtabs.h"

#include "dicomview/scenes/sets/frameset.h"

using namespace Sokar;

DicomTabs::DicomTabs(QWidget *parent) : QTabWidget(parent) {
	setTabsClosable(true);
	setAcceptDrops(true);

	connect(this, &DicomTabs::tabCloseRequested, this, &DicomTabs::removeDicomView);
}

void DicomTabs::addDicomFile(const gdcm::ImageReader *file) {

	auto sceneSet = new DicomFrameSet(file);
	auto dicomView = new DicomView(sceneSet, this);
	setCurrentIndex(addTab(dicomView, dicomView->getTitle()));
}


void DicomTabs::removeDicomView(int i) {
	auto view = widget(i);
	removeTab(i);
	delete view;
}


void DicomTabs::dropEvent(QDropEvent *event) {
	const auto *mimeData = event->mimeData();

	if (mimeData->hasUrls()) {

		for (auto &path : mimeData->urls()) {

			auto *ir = new gdcm::ImageReader;

			ir->SetFileName(path.toLocalFile().toStdString().c_str());

			if (!ir->Read()) {
				QMessageBox::critical(this, "Error", "An error has occured !");
				delete ir;
				continue;
			}

			Settings::bumpRecentOpen(path.path());

			addDicomFile(ir);
		}

		event->acceptProposedAction();
	}
}

void DicomTabs::dragEnterEvent(QDragEnterEvent *event) {
	event->acceptProposedAction();
}

void DicomTabs::dragMoveEvent(QDragMoveEvent *event) {
	event->acceptProposedAction();
}

void DicomTabs::dragLeaveEvent(QDragLeaveEvent *event) {
	event->accept();
}

DicomView *DicomTabs::currentDicomView() {
	return (DicomView *) currentWidget();
}
