#pragma once

#include <QObject>

namespace Sokar {
	class ImgWindow;
}

class Sokar::ImgWindow : public QObject {
Q_OBJECT
protected:

	ushort center, width, max;
	uchar *array = nullptr;

public:
	ImgWindow();

	~ImgWindow() override;

	ushort getCenter() const;

	void setCenter(ushort center);

	ushort getWidth() const;

	void setWidth(ushort width);

	void setMax(ushort max);

	ushort getMax() const;

	int getRightEdge();

	int getLeftEdge();

	void mvCenter(short d);

	void mvWidth(short d);

	void getLUT(uchar *&array, ushort &length);

	void genLUT();

signals:

	void centerChanged(ushort c1, ushort c2);

	void widthChanged(ushort w1, ushort w2);
};

