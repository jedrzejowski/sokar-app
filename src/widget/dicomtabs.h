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

		void addDicomFile(const gdcm::ImageReader *file);

		DicomView* currentDicomView();

	protected:

		void dropEvent(QDropEvent *event) override;
		void dragEnterEvent(QDragEnterEvent *event) override;
		void dragMoveEvent(QDragMoveEvent *event) override;
		void dragLeaveEvent(QDragLeaveEvent *event) override;

	private slots:
		void removeDicomView(int i);
	};
}

