#pragma once

#include <QtCore>
#include <QtWidgets>
#include <gdcmImage.h>
#include <gdcmImageReader.h>

#include "sokar/exception.h"

namespace Sokar {
	class DicomScene;
}

class Sokar::DicomScene : public QGraphicsScene {
Q_OBJECT

protected:
	const gdcm::Image &gdcmImage;
	const gdcm::File &gdcmFile;
	const gdcm::DataSet &gdcmDataSet;

	QPixmap pixmap;
	QGraphicsPixmapItem *pixmapItem = nullptr;
	QGraphicsTextItem *text11, *text12, *text13, *text21, *text23, *text31, *text32, *text33;

public:

	~DicomScene() override;

	static DicomScene *createForImg(const gdcm::ImageReader &imageReader);

	const gdcm::File& getGdcmFile() { return gdcmFile; }

protected:

	explicit DicomScene(const gdcm::File &gdcmFile, const gdcm::Image &gdcmImage);

	void reloadPixmap();

	virtual bool genQPixmap() = 0;

private:

	void initTexts();

public slots:

	void reposItems();
};
