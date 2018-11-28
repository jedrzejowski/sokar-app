#pragma once

#include <QtCore>
#include <QtWidgets>
#include "ui_dicomview.h"

#include "scenes/dicomsceneset.h"

namespace Ui {
	class DicomView;
}

namespace Sokar {
	class DicomView : public QWidget {
	Q_OBJECT

	private:
		Ui::DicomView *ui = nullptr;

		QVector<DicomSceneSet *> sceneSets;
		DicomSceneSet *curentSceneSet;

	public:
		explicit DicomView(QWidget *parent = nullptr);

		~DicomView();

		DicomScene *currentDicomScene() {
			return (DicomScene *) ui->graphicsView->scene();
		}

		void addDicomImage(const gdcm::ImageReader *reader);


	private slots:
		void activateScene(DicomScene *scene);

	};
}



