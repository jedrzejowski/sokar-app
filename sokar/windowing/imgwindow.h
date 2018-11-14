#pragma once

#include <QObject>

namespace Sokar {
	class ImgWindow;

	class ImgWindowINT;
	class ImgWindowUINT;
}

class Sokar::ImgWindow : public QObject {
Q_OBJECT
protected:

	int center, width, max, min;
	uchar *array = nullptr;

	int rescaleIntercept = 0, rescaleSlope = 0;

public:
	ImgWindow();

	~ImgWindow() override;

	int getCenter() const;

	virtual void setCenter(int center);

	int getWidth() const;

	virtual void setWidth(int width);

	void setMax(int max);

	int getMax() const;

	int getMin() const;

	void setMin(int min);

	int getRescaleIntercept() const;

	void setRescaleIntercept(int rescaleIntercept);

	int getRescaleSlope() const;

	void setRescaleSlope(int rescaleSlope);

	int getRightEdge();

	int getLeftEdge();

	void mvCenter(int d);

	void mvWidth(int d);

	void getLUT(uchar *&array, ushort &length);

	void genLUT();

signals:

	void centerChanged();

	void centerChanged(int c1, int c2);

	void widthChanged();

	void widthChanged(int w1, int w2);
};

