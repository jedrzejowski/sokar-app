#pragma once

#include "windowint.h"
#include <iostream>

namespace Sokar::Monochrome2 {
	class WindowIntStatic : public WindowInt {
	protected:
		std::vector<Pixel> array;
	public:

		Type type() override {
			return IntStatic;
		}

		bool genLUT() override {

			if (WindowInt::genLUT()) {

				if (array.size() != signedMove + maxValue)
					array.resize(signedMove + maxValue);

				//http://gcc.gnu.org/onlinedocs/gcc/_005f_005fint128.html
				//xD po całości, 8h na to zmarnowałem
				__int128 x = signedMove;
				x = ~x + 1;

				for (auto &pixel : array) {

					if (x < x0 || (hasBackground && x < backgroundLvl)) {
						pixel = Pixel(y0);
					} else if (x > x1) {
						pixel = Pixel(y1);
					} else {
						pixel = Pixel((quint8) (a * x + b));
					}

					x++;
				}

				return true;
			}
			return false;
		}

		inline const Pixel &getLUT(quint64 value) override {

			return array[signedMove + value];
		}
	};
}