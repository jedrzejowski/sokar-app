#pragma once

#include <QtCore>

namespace Sokar {
	class ImgWindow;
}

class Sokar::ImgWindow {
protected:

	ushort bumpsize = 1;
	ushort center, width, maxLeft, maxRight;

public:
	ImgWindow();

	ushort getCenter() const;

	void setCenter(ushort center);

	ushort getWidth() const;

	void setWidth(ushort width);

	ushort getMaxLeft() const;

	void setMaxLeft(ushort maxLeft);

	ushort getMaxRight() const;

	void setMaxRight(ushort maxRight);

	ushort getRightEdge();

	ushort getLeftEdge();

	void incCenter();

	void decCenter();

	void incSize();

	void decSize();
};

