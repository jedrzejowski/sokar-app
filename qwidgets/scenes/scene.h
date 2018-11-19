#pragma once

#include <QtCore>
#include <QtWidgets>
#include <gdcmImage.h>
#include <gdcmImageReader.h>
#include <gdcmStringFilter.h>
#include <qwidgets/qdicomgraphics.h>

#include "_classdef.h"
#include "sokar/exception.h"
#include "params.h"

class Sokar::DicomScene : public QGraphicsScene {
Q_OBJECT

protected:
	const gdcm::Image &gdcmImage;
	const gdcm::File &gdcmFile;
	const gdcm::DataSet &gdcmDataSet;

	gdcm::StringFilter gdcmStringFilter;

	SceneParams *sceneParams;

	QPixmap pixmap;
	QGraphicsPixmapItem *pixmapItem = nullptr;
	QGraphicsTextItem *text11, *text12, *text13, *text21, *text23, *text31, *text32, *text33;
	QColor textColor;

public:

	explicit DicomScene(const gdcm::ImageReader &imageReader, SceneParams *sceneParams);

	~DicomScene() override;

	static DicomScene *createForImg(const gdcm::ImageReader &imageReader, SceneParams *sceneParams);

	const gdcm::File &getGdcmFile() { return gdcmFile; }

	void reloadPixmap();

protected:

	virtual bool genQPixmap() = 0;

	QDicomGraphics *parentGraphics() const {
		return (QDicomGraphics *) this->parent();
	}

private:

	void initTexts();

	//region PixelSpacingIndicator
	void initPixelSpacingIndicator();
	//endregion

public slots:

	void refreshText33();

	virtual void reposItems();

protected:
	//region TextGen
	virtual QString genText33();
	//endregion

};
