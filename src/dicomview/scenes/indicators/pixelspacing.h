#pragma once

#include "_base.h"

namespace Sokar {
	class LineIndicator : public SceneIndicator {
	private:
		QPen pen;
		qreal pxLength = 0;
		qreal realWidth;
		QGraphicsTextItem *text;
		QVector<QGraphicsLineItem *> lines;

		void update();
	public:

		LineIndicator(DataConverter &dataConverter);

		void setText(QString str);
		qreal getPxLength() const;
		void setPxLength(qreal pxLength);
		qreal getRealWidth() const;
		qreal getRealHeight() const;

		void reposition() override;

	};

	class PixelSpacingIndicator : public SceneIndicator {
	private:

		qreal xSpacing, ySpacing;
		qreal xDim, yDim;

		LineIndicator xLine, yLine;


	public:
		PixelSpacingIndicator(DataConverter& dataConverter);

		//region Getter & Setters

		qreal getXSpacing() const;
		void setXSpacing(qreal xSpacing);
		qreal getYSpacing() const;
		void setYSpacing(qreal ySpacing);
		qreal getXDim() const;
		void setXDim(qreal xDim);
		qreal getYDim() const;
		void setYDim(qreal yDim);

		qreal getBottomSpace() override;
		qreal getRightSpace() override;

		//endregion

		void reposition() override;
		void updateLines();
	};
}
