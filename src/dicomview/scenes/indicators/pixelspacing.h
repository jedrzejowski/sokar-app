#pragma once

#include "_base.h"

namespace Sokar {
	class LineIndicator : public SceneIndicator {
	private:
		QPen pen;
		uint pxLength = 0;
		qreal realWidth;
		QGraphicsTextItem *text;
		QVector<QGraphicsLineItem *> lines;

		void update();
	public:

		LineIndicator(DataConverter &dataConverter);

		void setText(QString str);
		uint getPxLength() const;
		void setPxLength(uint pxLength);
		qreal getRealWidth() const;
		qreal getRealHeight() const;

		void reposition() override;

	};

	class PixelSpacingIndicator : public SceneIndicator {
	private:

		double xSpacing, ySpacing;
		uint xDim, yDim;

		LineIndicator xLine, yLine;

		void updateLines();

	public:
		PixelSpacingIndicator(DataConverter& dataConverter);

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
