#pragma once

#include <QtCore>
#include "../dicomscene.h"

namespace Sokar {
	class DicomImageScene : public DicomScene {
	Q_OBJECT
	protected:

		std::vector<char> originBuffer;
		std::vector<Pixel> targetBuffer;
		uint imgDimX, imgDimY;

		QImage qImage;
		QPixmap pixmap, iconPixmap;
		QGraphicsPixmapItem *pixmapItem = nullptr;

		QTransform panTransform, scaleTransform, centerTransform, rotateTransform;

		//region Indicators
		PixelSpacingIndicator *pixelSpacingIndicator = nullptr;
		ImageOrientationIndicator *imageOrientationIndicator = nullptr;
		void initPixelSpacingIndicator();
		void initImageOrientationIndicator();
		//endregion

	public:

		explicit DicomImageScene(SceneParams& params);

		inline const QPixmap *getPixmap() const { return &pixmap; }

		SceneAvatar *getAvatar();

		const QPixmap &getIcon();

	protected:
		virtual bool generatePixmap() = 0;

		void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

		virtual QTransform pixmapTransformation();

		void wheelEvent(QGraphicsSceneWheelEvent *event) override;

		bool isMovieModeAcceptable(MovieMode *movieMode) override;

	public slots:
		void reloadPixmap();
		void updatePixmapTransformation();
		void prepare();
		void attached();
	};
}