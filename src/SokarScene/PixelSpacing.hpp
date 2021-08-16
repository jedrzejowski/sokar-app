#pragma once

#include "SokarScene.hpp"
#include "SceneIndicator.hpp"

namespace SokarScene {
	class LineIndicator : public SceneIndicator {
	private:
		QPen pen;
		qreal pxLength = 0;
		qreal realWidth;
		QGraphicsTextItem *text;
		QVector<QGraphicsLineItem *> lines;

		void update();
	public:

		LineIndicator(SokarDicom::DataConverter &dataConverter);

		void setText(QString str);
		qreal getPxLength() const;
		void setPxLength(qreal pxLength);
		qreal getRealWidth() const;
		qreal getRealHeight() const;

		void reposition() override;
		bool isAlive() override;

	};

	class PixelSpacing : public SceneIndicator {
	private:

		qreal xSpacing, ySpacing;
		qreal xDim, yDim;
		qreal xScale = 1.0, yScale = 1.0;

		LineIndicator xLine, yLine;

	public:
	    explicit PixelSpacing(SokarDicom::DataConverter& dataConverter);

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


		void setScaleTransform(const QTransform &scaleTransform);

		//endregion

		void reposition() override;
		void updateLines();
		bool isAlive() override;
	};
}
