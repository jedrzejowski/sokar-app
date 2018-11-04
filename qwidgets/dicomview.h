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

protected:
	void resizeEvent(QResizeEvent *event) override;
};


