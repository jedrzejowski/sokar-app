#pragma once

#include <QtCore>
#include <QtWidgets>

#include <gdcmImageReader.h>

#include "dicomview/dicomview.h"

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
		void addDicomFile(const gdcm::ImageReader *file);
		void addDicomFiles(const QStringList &paths);
		void addDicomFiles(DicomReaderVec &files);

	private slots:
		void removeDicomView(int i);
	};
}

