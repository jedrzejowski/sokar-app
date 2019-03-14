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


	try {
		auto *bundle = DicomBundle::create(path);

		Settings::bumpRecentOpen(path);

		addDicomFile(bundle);
		return;
	} catch (IOException &e) {
		QMessageBox::critical(this, tr("I/O Error"),
							  tr("Error occured while reading file '%1'").arg(e.file));
	}
}

void DicomTabs::addDicomFile(const DicomBundle *bundle) {

	try {
		auto sceneSet = new DicomFrameSet(bundle, this);
		auto dicomView = new DicomView(sceneSet, this);
		addDicomView(dicomView);

		return;
	} catch (DicomTagParseError &e) {
	} catch (Exception &e) {
	}

	delete bundle;
}


void DicomTabs::addDicomFiles(const QStringList &paths) {
	gdcmBundleVec bundles;

	try {
		for (auto &path : paths) {

			auto *bundle = DicomBundle::create(path);

			Settings::bumpRecentOpen(path);

			bundles << bundle;
		}

		addDicomFiles(bundles);
	} catch (IOException &e) {
		QMessageBox::critical(this, tr("I/O Error"),
							  tr("Error occured while reading file '%1'").arg(e.file));

		for (auto &bundle : bundles)
			delete bundle;
	}
}

void DicomTabs::addDicomFiles(gdcmBundleVec &bundles) {

	try {
		for (auto sceneSet : DicomFileSet::create(bundles, this)) {
			auto dicomView = new DicomView(sceneSet, this);
			addDicomView(dicomView);
		}

		return;
	} catch (DicomTagParseError &e) {
	} catch (Exception &e) {
	}

	for (auto &bundle : bundles)
		delete bundle;
}

void DicomTabs::addDicomView(DicomView *dicomView) {
	setCurrentIndex(addTab(dicomView, dicomView->getTitle()));
}
