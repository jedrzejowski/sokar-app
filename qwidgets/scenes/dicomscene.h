#pragma once

#include <QtCore>
#include <QtWidgets>
#include <gdcmImage.h>
#include <gdcmImageReader.h>
#include <gdcmStringFilter.h>
#include <qwidgets/qdicomgraphics.h>


#include "sokar/pixel.h"
#include "sokar/exception.h"
#include "_classdef.h"
#include "scene.h"
#include "params.h"
#include "indicators/pixel.h"
#include "indicators/imgorientation.h"

class Sokar::DicomScene : public Sokar::Scene {
Q_OBJECT

protected:
	const gdcm::Image &gdcmImage;
	const gdcm::File &gdcmFile;
	const gdcm::DataSet &gdcmDataSet;
	gdcm::StringFilter gdcmStringFilter;

	static const int Z_IMAGE = 0;
	static const int Z_INDICATOR = 10;

	std::vector<char> originBuffer;
	std::vector<Pixel> targetBuffer;
	uint dimX, dimY;
	quint64 area;

	QPixmap pixmap;
	QGraphicsPixmapItem *pixmapItem = nullptr;

//region Indicators
private:
	void initIndicators();

	PixelSpacingIndicator *pixelSpacingIndicator;
	ImageOrientationIndicator *imageOrientationIndicator;

	void initPixelSpacingIndicator();
	void initImageOrientationIndicator();
//endregion

public:

	explicit DicomScene(SceneParams &sceneParams);

	~DicomScene() override;

	const gdcm::File &getGdcmFile() { return gdcmFile; }

	void reloadPixmap();

protected:

	virtual bool genQPixmap() = 0;

	QDicomGraphics *parentGraphics() const {
		return (QDicomGraphics *) this->parent();
	}


public slots:

	void reposItems() override;

};
