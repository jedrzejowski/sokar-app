#pragma once

#include <QtCore>
#include <QtWidgets>
#include <gdcmImage.h>

#include "convert/convert.h"
#include "sokar/exception.h"

namespace Sokar {
	class DicomScene;
}

class Sokar::DicomScene : public QGraphicsScene {
Q_OBJECT

private:
	gdcm::Image *gdcmImage = nullptr;

	QGraphicsTextItem *text11, *text12, *text13, *text21, *text23, *text31, *text32, *text33;

	int maxWidth, maxHeight;

public:

	~DicomScene() override;

	void resize(int, int);

	static DicomScene *createForImg(gdcm::Image *gdcmImage);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

	void wheelEvent(QGraphicsSceneWheelEvent *event) override;

	void refreshPixMap();

	virtual QPixmap* genQPixmap() = 0;

private:
	explicit DicomScene(gdcm::Image *gdcmImage);

	void initTexts();

	void positTexts();
};
