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

protected:
	gdcm::Image *gdcmImage = nullptr;
	QGraphicsPixmapItem *curentPixmapItem = nullptr;

	QGraphicsTextItem *text11, *text12, *text13, *text21, *text23, *text31, *text32, *text33;

	int maxWidth, maxHeight;

public:

	~DicomScene() override;

	void resize(int, int);

	static DicomScene *createForImg(gdcm::Image *gdcmImage);

protected:

	explicit DicomScene(gdcm::Image *gdcmImage);

	void reloadPixmap();

	virtual const QPixmap genQPixmap() = 0;

private:

	void initTexts();

	void reposItems();

};
