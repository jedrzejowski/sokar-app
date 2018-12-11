#pragma once

#include <QtCore>
#include <QtWidgets>

#include <gdcmImageReader.h>

namespace Sokar {
	class DicomTabs : public QTabWidget {
	Q_OBJECT
	public:
		explicit DicomTabs(QWidget *parent = nullptr);

		void addDicomFile(gdcm::ImageReader *file);

	protected:

		void dropEvent(QDropEvent *event) override;
		void dragEnterEvent(QDragEnterEvent *event) override;
		void dragMoveEvent(QDragMoveEvent *event) override;
		void dragLeaveEvent(QDragLeaveEvent *event) override;

	private slots:
		void removeDicomView(int i);
	};
}

