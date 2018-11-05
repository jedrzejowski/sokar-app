#pragma once

#include <QtCore>

namespace Sokar {
	class ImgWindow;
}

class Sokar::ImgWindow {
private:
	qint16 bumpsize = 1;
	qint16 center, size;
public:
	ImgWindow();

	void incCenter();

	void decCenter();

	void incSize();

	void decSize();
};

