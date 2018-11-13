#pragma once

#include <QtCore>
#include <QtWidgets>
#include <gdcmImage.h>
#include <gdcmImageReader.h>

#include "sokar/exception.h"
#include "sokar/sceneparams.h"

namespace Sokar {
	class DicomScene;
}

class Sokar::DicomScene : public QGraphicsScene {
Q_OBJECT

protected:
	const gdcm::Image &gdcmImage;
	const gdcm::File &gdcmFile;
	const gdcm::DataSet &gdcmDataSet;

	SceneParams* sceneParams;

	QPixmap pixmap;
	QGraphicsPixmapItem *pixmapItem = nullptr;
	QGraphicsTextItem *text11, *text12, *text13, *text21, *text23, *text31, *text32, *text33;


public:

	explicit DicomScene(const gdcm::ImageReader &imageReader, SceneParams *sceneParams);
	~DicomScene() override;

	static DicomScene *createForImg(const gdcm::ImageReader &imageReader, SceneParams* sceneParams);

	const gdcm::File &getGdcmFile() { return gdcmFile; }

	void reloadPixmap();

protected:

	virtual bool genQPixmap() = 0;

private:

	void initTexts();

public slots:

	void refreshText33();
	void reposItems();
};
