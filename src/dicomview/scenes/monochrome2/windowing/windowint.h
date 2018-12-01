#pragma once

#include <QMenu>

#include "_window.h"


namespace Sokar::Monochrome2 {

	class WindowInt : public Window {
	Q_OBJECT

	private:
		struct DefaultWindow {
			__int128 center;
			__int128 width;
			QString name = "";
		};

	protected:
		QGraphicsTextItem *text;
		QList<DefaultWindow> defaultWindows;

		bool hasBackground = false;

		__int128 center, width, backgroundLvl;
		double rescaleIntercept = 0, rescaleSlope = 1;

		quint64 signedMove = 0;
		quint64 maxValue = 0;

		__int128 x1, x0;
		double a, b;

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

		inline double getRescaleIntercept() const {
			return rescaleIntercept;
		}

		void setRescaleIntercept(double rescaleIntercept);

		inline double getRescaleSlope() const {
			return rescaleSlope;
		}

		void setRescaleSlope(double rescaleSlope);

		inline quint64 getMaxValue() const {
			return maxValue;
		}

		void setMaxValue(quint64 length);

		inline bool isSigned() const {
			return signedMove > 0;
		}

		void setSigned(bool isSigned);

		inline __int128_t getBackgroundLvl() const {
			return backgroundLvl;
		}

		void setBackgroundLvl(__int128_t backgroundLvl);

		//endregion

		void pushDefaultValues(__int128 center, __int128 width, QString name = "");


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