#include "dicomtabs.h"
#include "dicomview/dicomview.h"

using namespace Sokar;

DicomTabs::DicomTabs(QWidget *parent) : QTabWidget(parent) {
	setTabsClosable(true);
	setAcceptDrops(true);

	connect(this, &DicomTabs::tabCloseRequested, this, &DicomTabs::removeDicomView);
}

void DicomTabs::addDicomFile(gdcm::ImageReader *file) {

	auto dicomView = new DicomView(this);
	dicomView->addDicomFile(file);
	addTab(dicomView, tr("Jupiii"));
}


void DicomTabs::removeDicomView(int i) {
	auto view = widget(i);
	removeTab(i);
	delete view;
}


void DicomTabs::dropEvent(QDropEvent *event) {
	qDebug("dropEvent");
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
	qDebug("dragEnterEvent");
}

void DicomTabs::dragMoveEvent(QDragMoveEvent *event) {
	event->acceptProposedAction();
	qDebug("dragMoveEvent");
}

void DicomTabs::dragLeaveEvent(QDragLeaveEvent *event) {
	event->accept();
	qDebug("dragLeaveEvent");
}
