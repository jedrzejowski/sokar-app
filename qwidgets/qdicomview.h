#pragma once

#include "dicomscene.h"

namespace Ui {
	class QDicomView;
}

namespace Sokar {
	class QDicomView;
}

class Sokar::QDicomView : public QWidget {
Q_OBJECT

private:
	Ui::QDicomView *ui = nullptr;
	std::vector<DicomScene *> scenes;

public:
	explicit QDicomView(QWidget *parent = nullptr);

	~QDicomView();

	DicomScene *currentDicomScene();

	void addDicomImage(const gdcm::ImageReader &reader);


protected:
	// Aby wyłączyć scrolowanie
//	void scrollContentsBy(int dx, int dy) override;

	void resizeEvent(QResizeEvent *event) override;

	void activateScene(DicomScene *scene);

};


