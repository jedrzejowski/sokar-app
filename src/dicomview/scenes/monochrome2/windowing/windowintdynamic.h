#pragma once

#include "windowint.h"

namespace Sokar::Monochrome2 {
	class WindowIntDynamic: public WindowInt{
	public:
		Type type() override {
			return IntDynamic;
		}

		inline const Pixel &getLUT(quint64 value) override {


			return Pixel((quint8) (a * (value + signedMove) + b));
		}
	};
}