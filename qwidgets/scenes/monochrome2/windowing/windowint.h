#pragma once

#include <QMenu>

#include "_window.h"


namespace Sokar::Monochrome2 {

	class WindowInt : public Window {
	Q_OBJECT

	protected:
		QMenu qmenu;
		QGraphicsTextItem *text;

		__int128 center, width;
		__int128 rescaleIntercept = 0, rescaleSlope = 1;

		quint64 signedMove = 0;
		quint64 maxValue = 0;

		__int128 x1, x0;
		quint8 y0 = 0, y1 = 255;
		float a, b;

	private:
		void regenText();

	public:

		WindowInt();

		//region Getters & Setters

		inline __int128 getCenter() const {
			return center;
		}

		void setCenter(__int128 center);

		inline __int128 getWidth() const {
			return width;
		}

		void setWidth(__int128 width);

		inline __int128 getRescaleIntercept() const {
			return rescaleIntercept;
		}

		void setRescaleIntercept(__int128 rescaleIntercept);

		inline __int128 getRescaleSlope() const {
			return rescaleSlope;
		}

		void setRescaleSlope(__int128 rescaleSlope);

		inline quint64 getMaxValue() const {
			return maxValue;
		}

		void setMaxValue(quint64 length);

		inline bool isSigned() const {
			return signedMove > 0;
		}

		void setSigned(bool isSigned);

		//endregion

		inline void mvHorizontal(int v) override {
			setWidth(getWidth() + v);
		}

		inline void mvVertical(int v) override {
			setCenter(getCenter() + v);
		}

		void genLUT() override;

		void selectWindowingIndicator(QGraphicsSceneMouseEvent* event);

		void reposition() override;

	signals:

		void centerChanged();

		void widthChanged();

	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	};
}