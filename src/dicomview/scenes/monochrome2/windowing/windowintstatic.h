#pragma once

#include "windowint.h"

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

				__int128 x = -signedMove;

				for (auto &pixel : array) {

					if (x > x1) {
						pixel = Pixel(y1);

					} else if (x < x0) {
						pixel = Pixel(y0);
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
			if (value < signedMove || value > maxValue)
				qDebug() << value;
			return array[signedMove + value];
		}
	};
}