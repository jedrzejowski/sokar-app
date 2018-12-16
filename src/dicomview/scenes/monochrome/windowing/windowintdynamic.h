#pragma once

#include "windowint.h"

namespace Sokar::Monochrome {
	class WindowIntDynamic : public WindowInt {
	private:
		Pixel background, foreground;
	public:

		WindowIntDynamic(DataConverter &dataConverter) : WindowInt(dataConverter) {}

		Type type() override {
			return IntDynamic;
		}

		bool genLUT() override {

			if (WindowInt::genLUT()) {

				background = isInversed() ? palette->getForeground() : palette->getBackground();
				foreground = isInversed() ? palette->getBackground() : palette->getForeground();

				return true;
			}
			return false;
		}

		inline const Pixel &getLUT(quint64 value) override {
			value += signedMove;

			if (value < x0) {
				return background;
			} else if (value > x1) {
				return foreground;
			} else {
				return palette->getPixel(a * value + b);
			}
		}
	};
}