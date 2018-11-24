#pragma once

#include <QtCore>
#include "ui_dicomview.h"

#include "scenes/dicomsceneset.h"

namespace Ui {
	class DicomView;
}

namespace Sokar {
	class DicomView;
}

class Sokar::DicomView : public QWidget {
Q_OBJECT

private:
	Ui::DicomView *ui = nullptr;
	std::vector<DicomSceneSet *> files;

public:
	explicit DicomView(QWidget *parent = nullptr);

	~DicomView();

	DicomScene *currentDicomScene() {
		return (DicomScene *) ui->graphicsView->scene();
	}

	void addDicomImage(const gdcm::ImageReader *reader);


protected:

	void resizeEvent(QResizeEvent *event) override;

	void activateScene(DicomScene *scene);

};


