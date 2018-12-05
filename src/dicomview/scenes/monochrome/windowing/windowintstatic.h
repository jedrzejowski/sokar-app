#pragma once

#include "windowint.h"
#include <iostream>

namespace Sokar::Monochrome {
	class WindowIntStatic : public WindowInt {
	protected:
//		std::vector<Pixel> array;
		Pixel *array = nullptr;
		quint64 arraySize = 0;
	public:
		~WindowIntStatic() override {
			delete array;
		}

		Type type() override {
			return IntStatic;
		}

		bool genLUT() override {

			if (WindowInt::genLUT()) {

//				if (array.size() != signedMove + maxValue)
//					array.resize(signedMove + maxValue);

				if (arraySize != signedMove + maxValue) {
					delete array;
					arraySize = signedMove + maxValue;
					array = new Pixel[arraySize];
				}

				//http://gcc.gnu.org/onlinedocs/gcc/_005f_005fint128.html
				//xD po całości, 8h na to zmarnowałem
				__int128 x = signedMove;
				x = ~x + 1;

				auto background = isInversed() ? palette->getForeground() : palette->getBackground();
				auto foreground = isInversed() ? palette->getBackground() : palette->getForeground();

//				for (auto &pixel : array) {
				Pixel *pixel = array;
				for (int i = 0; i <= arraySize; i++) {

					if (x < x0) {
						*pixel = background;
					} else if (x > x1) {
						*pixel = foreground;
					} else {
						*pixel = palette->getPixel(a * x + b);
					}

					x++;
					pixel++;
				}

				return true;
			}
			return false;
		}

		inline const Pixel &getLUT(quint64 value) override {
			return *(array + signedMove + value);
		}
	};
}