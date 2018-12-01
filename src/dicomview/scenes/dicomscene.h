#pragma once

#include <QtCore>
#include <QtWidgets>

#include <gdcmImage.h>
#include <gdcmImageReader.h>
#include <gdcmStringFilter.h>

#include "dicomview/_classdef.h"
#include "dicomview/graphics.h"
#include "dicomview/toolbar.h"

#include "sokar/pixel.h"
#include "sokar/exception.h"
#include "../_classdef.h"
#include "scene.h"
#include "params.h"
#include "sceneavatar.h"
#include "indicators/pixel.h"
#include "indicators/imgorientation.h"

namespace Sokar {
	class DicomScene : public Scene {
	Q_OBJECT

	protected:

		const gdcm::Image &gdcmImage;
		const gdcm::File &gdcmFile;
		const gdcm::DataSet &gdcmDataSet;
		gdcm::StringFilter gdcmStringFilter;

		std::vector<char> originBuffer;
		std::vector<Pixel> targetBuffer;
		uint dimX, dimY;
		quint64 area;

		QPixmap pixmap, iconPixmap;
		QGraphicsPixmapItem *pixmapItem = nullptr;

		QTransform panTransform, scaleTransform, centerTransformat, rotateTransform;

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

		//region Getters

		inline const gdcm::File &getGdcmFile() { return gdcmFile; }

		inline const QPixmap *getPixmap() const { return &pixmap; }

		SceneAvatar *getAvatar();

		const QPixmap &getIcon();

		inline DicomSceneSet *getDicomSceneSet() { return (DicomSceneSet *) this->parent(); }

		inline DicomView *getDicomView() {
			return (DicomView *) this->parent()->parent();
		}

		//endregion

		void reloadPixmap();

		virtual void customizeToolBar(DicomToolBar *toolBar);

	protected:
		virtual bool generatePixmap() = 0;

		void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

		virtual QTransform pixmapTransformation();

		void wheelEvent(QGraphicsSceneWheelEvent *event) override;

	public slots:
		void toolBarActionSlot(DicomToolBar::Action action);
		void reposItems() override;
		void updatePixmapTransformation();

	};
}