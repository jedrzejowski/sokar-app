#include "dicomtabs.h"
#include "dicomview/dicomview.h"

using namespace Sokar;

DicomTabs::DicomTabs(QWidget *parent) : QTabWidget(parent) {
	setTabsClosable(true);
	setAcceptDrops(true);

	connect(this, &DicomTabs::tabCloseRequested, this, &DicomTabs::removeDicomView);
}

void DicomTabs::addDicomFile(const gdcm::ImageReader *file) {

	auto dicomView = new DicomView(file, this);
	addTab(dicomView, dicomView->getTitle());
}


void DicomTabs::removeDicomView(int i) {
	auto view = widget(i);
	removeTab(i);
	delete view;
}


void DicomTabs::dropEvent(QDropEvent *event) {
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
