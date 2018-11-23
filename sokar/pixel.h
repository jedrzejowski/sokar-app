#pragma once

#include <qplatformdefs.h>

namespace Sokar {
	struct Pixel;
}

struct Sokar::Pixel {
	uint8_t red = 0, green = 0, blue = 0;

	Pixel() {}

	Pixel(uint8_t gray) : red(gray), green(gray), blue(gray) {}

	Pixel(uint8_t red, uint8_t green, uint8_t blue) : red(red), green(green), blue(blue) {}
};
