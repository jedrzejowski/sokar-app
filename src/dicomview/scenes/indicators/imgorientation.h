#pragma once

#include "_base.h"

namespace Sokar {
	class ImageOrientationIndicator : public SceneIndicator {
	private:
		QGraphicsTextItem *topText, *leftText, *rightText, *bottomText;

		QMatrix4x4 imgMatrix;
		QMatrix4x4 rotateTransform;

		struct {
			QVector4D right, left, head, feet, anterior, posterior;
		} scenePosition;

	public:
		ImageOrientationIndicator();

		void setRotateTransform(QTransform &rotateTransform);
		void setOrientation(QString orient);

		void reposition() override;

	public slots:
		void update();
	};
}