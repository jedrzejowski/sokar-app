#pragma once

#include <QtWidgets/QGraphicsView>
#include "dicomscene.h"

namespace Sokar {
	class DicomView;
}

class Sokar::DicomView : public QGraphicsView {
Q_OBJECT
public:
	explicit DicomView(QWidget *parent = nullptr);

	DicomScene *dicomScene();

	void setDicomScene(DicomScene *scene);


protected:
	// Aby wyłączyć scrolowanie
	void scrollContentsBy(int dx, int dy) override;

	void resizeEvent(QResizeEvent *event) override;

};


