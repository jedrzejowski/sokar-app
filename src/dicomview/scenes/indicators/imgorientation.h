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

		void initData();
	public:

		ImageOrientationIndicator(DataConverter &dataConverter);

		void setRotateTransform(const QTransform &rotateTransform);

		void reposition() override;
		bool isAlive() override;
	public slots:
		void update();
	};
}