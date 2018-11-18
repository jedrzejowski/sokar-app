#pragma once

#include <qplatformdefs.h>

namespace Sokar {
	struct Pixel;
}

struct Sokar::Pixel {
	uchar red = 0, green = 0, blue = 0;

	Pixel() {}

	Pixel(uchar gray) : red(gray), green(gray), blue(gray) {}

	Pixel(uchar red, uchar green, uchar blue) : red(red), green(green), blue(blue) {}
};
