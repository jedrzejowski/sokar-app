#pragma once

#include <QMenu>

#include "_window.h"

namespace Sokar::Monochrome {

	class WindowInt : public Window {
	Q_OBJECT

	private:
		struct DefaultWindow {
			qreal center;
			qreal width;
			QString name = "";
		};

	protected:
		QGraphicsTextItem *text;
		QList<DefaultWindow> defaultWindows;

		bool hasBackground = false;

		qreal center, width, backgroundLvl;
		double rescaleIntercept = 0, rescaleSlope = 1;

		quint64 signedMove = 0;
		quint64 maxValue = 0;

		double x1, x0;
		double a, b;

	public:

		explicit WindowInt(SokarDicom::DataConverter &dataConverter);

		//region Getters & Setters

		inline qreal getCenter() const { return center; }

		void setCenter(qreal center);

		inline qreal getWidth() const { return width; }

		void setWidth(qreal width);

		inline double getRescaleIntercept() const { return rescaleIntercept; }

		void setRescaleIntercept(double rescaleIntercept);

		inline double getRescaleSlope() const { return rescaleSlope; }

		void setRescaleSlope(double rescaleSlope);

		inline quint64 getMaxValue() const { return maxValue; }

		void setMaxValue(quint64 length);

		inline bool isSigned() const { return signedMove > 0; }

		void setSigned(bool isSigned);

		inline qreal getBackgroundLvl() const { return backgroundLvl; }

		void setBackgroundLvl(qreal backgroundLvl);

		//endregion

		void pushDefaultValues(qreal center, qreal width, QString name = "");


		inline void mvHorizontal(int v) override {
			setWidth(getWidth() + v);
		}

		inline void mvVertical(int v) override {
			setCenter(getCenter() + v);
		}

		bool genLUT() override;

		void reposition() override;

	private:
		void regenText();

		void activateDefWin(const DefaultWindow &win);
	signals:

		void centerChanged();

		void widthChanged();

	protected:

		void genMenu() override;
	};
}