#pragma once

#include "windowint.h"
#include <iostream>

namespace Sokar::Monochrome {
	class WindowIntStatic : public WindowInt {
	protected:
		std::vector<Pixel> arrayVector;
		Pixel *pixelArray = nullptr;
		quint64 arraySize = 0;
	public:

		WindowIntStatic(DataConverter &dataConverter) : WindowInt(dataConverter) {}


		Type type() override {
			return IntStatic;
		}

		bool genLUT() override {

			if (WindowInt::genLUT()) {

//				if (array.size() != signedMove + maxValue)
//					array.resize(signedMove + maxValue);

				if (arraySize != signedMove + maxValue) {
					arraySize = signedMove + maxValue;
					arrayVector.resize(arraySize);
				}

				TrueInt x = -signedMove;

				auto &background = isInversed() ? palette->getForeground() : palette->getBackground();
				auto &foreground = isInversed() ? palette->getBackground() : palette->getForeground();

//				for (auto &pixel : array) {
				pixelArray = &arrayVector[0];
				for (int i = 0; i <= arraySize; i++) {

					if (x < x0) {
						*pixelArray = background;
					} else if (x > x1) {
						*pixelArray = foreground;
					} else {
						*pixelArray = palette->getPixel(a * x + b);
					}

					x++;
					pixelArray++;
				}

				pixelArray = &arrayVector[0];

				updateLastChange();

				return true;
			}
			return false;
		}

		inline const Pixel &getPixel(quint64 value) override {
			return *(pixelArray + signedMove + value);
		}
	};
}