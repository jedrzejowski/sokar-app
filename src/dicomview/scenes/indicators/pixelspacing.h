#pragma once

#include "_base.h"

namespace Sokar {
	class PixelSpacingIndicator : public SceneIndicator {
	private:
		QPen pen;
		QTransform transform;

		double xSpacing, ySpacing;
		uint xDim, yDim;

		QGraphicsTextItem xText, yText;

		QGraphicsItemGroup xLine, yLine;
		std::vector<QGraphicsLineItem> xLines, yLines;

		void updateTextX();
		void updateTextY();

	public:
		PixelSpacingIndicator();

		//region Getter & Setters

		double getXSpacing() const;
		void setXSpacing(double xSpacing);
		double getYSpacing() const;
		void setYSpacing(double ySpacing);
		uint getXDim() const;
		void setXDim(uint xDim);
		uint getYDim() const;
		void setYDim(uint yDim);

		qreal getBottomSpace() override;
		qreal getRightSpace() override;

		//endregion

		void reposition() override;
	};
}
