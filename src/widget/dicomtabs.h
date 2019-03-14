#pragma once

#include <QtCore>
#include <QtWidgets>

#include <gdcmImageReader.h>

#include "dicomview/dicomview.h"
#include "sokar/dicombundle.h"

namespace Sokar {
	class DicomTabs : public QTabWidget {
	Q_OBJECT
	public:
		explicit DicomTabs(QWidget *parent = nullptr);

		DicomView *currentDicomView();

	protected:

		void dropEvent(QDropEvent *event) override;
		void dragEnterEvent(QDragEnterEvent *event) override;
		void dragMoveEvent(QDragMoveEvent *event) override;
		void dragLeaveEvent(QDragLeaveEvent *event) override;

	public slots:
		void addDicomFile(const QString &path);
		void addDicomFile(const DicomBundle* bundle);
		void addDicomFiles(const QStringList &paths);
		void addDicomFiles(gdcmBundleVec &bundles);
		void addDicomView(DicomView* dicomView);

	private slots:
		void removeDicomView(int i);
	};
}

