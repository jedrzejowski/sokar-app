#pragma once

#include "imgwindow.h"

namespace Sokar {

	class ImgWindowINT;
}

class Sokar::ImgWindowINT : public Sokar::ImgWindow {
Q_OBJECT
protected:

	__int128 center, width;

	__int128 rescaleIntercept = 0, rescaleSlope = 1;

public:
	ImgWindowINT();

	~ImgWindowINT() override;

	__int128 getCenter() const;

	virtual void setCenter(__int128 center);

	__int128 getWidth() const;

	virtual void setWidth(__int128 width);

	__int128 getRescaleIntercept() const;

	void setRescaleIntercept(__int128 rescaleIntercept);

	__int128 getRescaleSlope() const;

	void setRescaleSlope(__int128 rescaleSlope);

	inline virtual const Pixel &getLUT(__int128 value) {
		return array[value];
	}

	virtual void genLUT();

signals:

	void centerChanged();

	void widthChanged();
};