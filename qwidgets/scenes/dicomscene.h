#pragma once

#include <QtCore>
#include <QtWidgets>
#include <gdcmImage.h>
#include <gdcmImageReader.h>
#include <gdcmStringFilter.h>
#include <qwidgets/qdicomgraphics.h>

#include "_classdef.h"
#include "scene.h"
#include "sokar/exception.h"
#include "params.h"

class Sokar::DicomScene : public Sokar::Scene {
Q_OBJECT
private:
	QGraphicsLineItem *pixelSpacingLineX, *pixelSpacingLineY;
	QGraphicsTextItem *pixelSpacingTextX, *pixelSpacingTextY;

protected:
	const gdcm::Image &gdcmImage;
	const gdcm::File &gdcmFile;
	const gdcm::DataSet &gdcmDataSet;

	static const int Z_IMAGE = 0;
	static const int Z_INDICATOR = 10;

	gdcm::StringFilter gdcmStringFilter;

	SceneParams *sceneParams;

	QPixmap pixmap;
	QGraphicsPixmapItem *pixmapItem = nullptr;
	QGraphicsTextItem *text11, *text12, *text13, *text21, *text31, *text33;
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

	void reposPixelSpacingIndicator();
	void initPixelSpacingIndicator();

public slots:

	void refreshText33();

	virtual void reposItems();

protected:
	//region TextGen
	virtual QString genText33();
	//endregion

};
