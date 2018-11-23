#pragma once

#include "_base.h"

namespace Sokar {
	class ImageOrientationIndicator : public SceneIndicator {
	private:
		QGraphicsTextItem *topText, *leftText, *rightText, *bottomText;

		static QString Left, Right, Anterior, Posterior, Feet, Head;

	public:
		ImageOrientationIndicator();

		void setOrientation(QString orient);
		void setAngle(qreal angle);

		void reposition() override;
	};
}