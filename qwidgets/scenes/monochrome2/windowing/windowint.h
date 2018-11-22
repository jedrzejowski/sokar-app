#pragma once

#include "_window.h"


namespace Sokar::Monochrome2 {

	class WindowINT : public Window {
	Q_OBJECT
	protected:

		__int128 center, width;
		__int128 rescaleIntercept = 0, rescaleSlope = 1;

		__int128 signedMove = 0;

		int length = 0;
		Pixel *array = nullptr;

	public:

		WindowINT();

		~WindowINT() override;

		__int128 getCenter() const;

		void setCenter(__int128 center);

		__int128 getWidth() const;

		void setWidth(__int128 width);

		__int128 getRescaleIntercept() const;

		void setRescaleIntercept(__int128 rescaleIntercept);

		__int128 getRescaleSlope() const;

		void setRescaleSlope(__int128 rescaleSlope);

		int getLength() const;

		void setLength(int length);

		bool isSigned() const;

		void setSigned(bool isSigned);

		inline virtual const Pixel &getLUT(__int128 value) {
			return array[value];
		}

		virtual void genLUT();

		void selectWindowingIndicator(QWidget *parent, QPoint position);

	signals:

		void centerChanged();

		void widthChanged();
	};
}