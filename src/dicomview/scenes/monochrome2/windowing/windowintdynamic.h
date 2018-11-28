#pragma once

#include "windowint.h"

namespace Sokar::Monochrome2 {
	class WindowIntDynamic: public WindowInt{
	public:
		Type type() override {
			return IntDynamic;
		}

		inline const Pixel &getLUT(quint64 value) override {
			if (value > x1)
				return Pixel(y1);

			if (value < x0)
				return Pixel(y0);

			return Pixel((quint8) (a * (value + signedMove) + b));
		}
	};
}