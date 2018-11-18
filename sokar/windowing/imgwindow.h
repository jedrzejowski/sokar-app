#pragma once

#include <QObject>
#include <sokar/pixel.h>

namespace Sokar {
	class ImgWindow;
}

class Sokar::ImgWindow : public QObject {
Q_OBJECT
protected:
	ushort maxBit;

	int length = 0;
	Pixel *array = nullptr;

public:
	ImgWindow();
	~ImgWindow();

	ushort getMaxBit() const;

	virtual void setMaxBit(ushort maxBit);
};

