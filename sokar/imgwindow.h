#pragma once

#include <QtCore>

namespace Sokar {
	class ImgWindow;
}

class Sokar::ImgWindow {
protected:

	ushort bumpsize = 1;
	ushort center, width, max;

public:
	ImgWindow();

	ushort getCenter() const;

	void setCenter(ushort center);

	ushort getWidth() const;

	void setWidth(ushort width);

	void setMax(ushort max);

	ushort getMax() const;

	int getRightEdge();

	int getLeftEdge();

	void incCenter();

	void decCenter();

	void incSize();

	void decSize();

	void genLUT(uchar* &array, ushort &length);
};

