#include "sokar/settings.h"
#include "dicomtabs.h"

#include "dicomview/scenes/sets/fileset.h"

using namespace Sokar;

DicomTabs::DicomTabs(QWidget *parent) : QTabWidget(parent) {
	setTabsClosable(true);
	setAcceptDrops(true);

	connect(this, &DicomTabs::tabCloseRequested, this, &DicomTabs::removeDicomView);
}

void DicomTabs::removeDicomView(int i) {
	auto view = widget(i);
	removeTab(i);
	delete view;
}

void DicomTabs::dropEvent(QDropEvent *event) {
	const auto *mimeData = event->mimeData();

	if (mimeData->hasUrls()) {
		event->acceptProposedAction();

		auto urls = mimeData->urls();

		if (urls.size() == 1) {

			addDicomFile(urls[0].toLocalFile());

		} else {

			QStringList paths;

			for (auto &path : urls)
				paths << path.toLocalFile();
			addDicomFiles(paths);

		}
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

void DicomTabs::addDicomFile(const QString &path) {

	auto *reader = new gdcm::ImageReader;

	try {

		reader->SetFileName(path.toStdString().c_str());

		if (!reader->Read()) {
			delete reader;
			throw IOException(path);
		}

		Settings::bumpRecentOpen(path);

		addDicomFile(reader);
		return;
	} catch (IOException &e) {
		QMessageBox::critical(this, tr("I/O Error"),
							  tr("Error occured while reading file '%1'").arg(e.file));
	}

	delete reader;
}

void DicomTabs::addDicomFile(const gdcm::ImageReader *reader) {

	try {
		auto sceneSet = new DicomFrameSet(reader, this);
		auto dicomView = new DicomView(sceneSet, this);
		setCurrentIndex(addTab(dicomView, dicomView->getTitle()));

		return;
	} catch (DicomTagParseError &e) {

	} catch (Exception &e) {

	}

	delete reader;
}


void DicomTabs::addDicomFiles(const QStringList &paths) {
	DicomReaderVec readers;

	try {
		for (auto &path : paths) {

			auto *reader = new gdcm::ImageReader;

			reader->SetFileName(path.toStdString().c_str());

			if (!reader->Read()) {
				delete reader;
				throw IOException(path);
			}

			Settings::bumpRecentOpen(path);

			readers << reader;
		}

		addDicomFiles(readers);
		return;
	} catch (IOException &e) {
		QMessageBox::critical(this, tr("I/O Error"),
							  tr("Error occured while reading file '%1'").arg(e.file));
	}

	for (auto &reader : readers)
		delete reader;
}

void DicomTabs::addDicomFiles(DicomReaderVec &readers) {

	try {
		auto sceneSet = new DicomFileSet(readers, this);
		auto dicomView = new DicomView(sceneSet, this);
		setCurrentIndex(addTab(dicomView, dicomView->getTitle()));

		return;
	} catch (DicomTagParseError &e) {

	} catch (Exception &e) {

	}

	for (auto &reader : readers)
		delete reader;
}
