#pragma once

#include <QtCore>
#include <QtWidgets>
#include <gdcmImage.h>
#include <gdcmImageReader.h>

#include "convert/convert.h"
#include "sokar/exception.h"

namespace Sokar {
	class DicomScene;
}

class Sokar::DicomScene : public QGraphicsScene {
Q_OBJECT

protected:
	gdcm::Image gdcmImage;
	gdcm::File gdcmFile;
	QPixmap *pixmap = nullptr;
	QGraphicsPixmapItem *pixmapItem = nullptr;

	QGraphicsTextItem *text11, *text12, *text13, *text21, *text23, *text31, *text32, *text33;

public:

	~DicomScene() override;

	static DicomScene *createForImg(const gdcm::ImageReader &imageReader);

protected:

	explicit DicomScene(gdcm::File &gdcmFile, gdcm::Image &gdcmImage);

	void reloadPixmap();

	virtual bool genQPixmap() = 0;

private:
	
	void initTexts();

public slots:

	void reposItems();
};
