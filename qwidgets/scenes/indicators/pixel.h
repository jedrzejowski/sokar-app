#pragma once

#include "_base.h"

namespace Sokar {
	class PixelSpacingIndicator : public SceneIndicator {
	private:
		QPen pen;
		QTransform transform;

		double xSpacing, ySpacing;
		uint xDim, yDim;

		QGraphicsTextItem *xText, *yText;
		QGraphicsLineItem *xLine, *yLine;

		void updateTextX();
		void updateTextY();

	public:
		PixelSpacingIndicator();

		double getXSpacing() const;

		void setXSpacing(double xSpacing);

		double getYSpacing() const;

		void setYSpacing(double ySpacing);

		uint getXDim() const;

		void setXDim(uint xDim);

		uint getYDim() const;

		void setYDim(uint yDim);

		void reposition() override;
	};
}
