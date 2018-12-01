#pragma once

#include <QObject>

#include "sokar/pixel.h"
#include "../../indicators/_base.h"

namespace Sokar::Monochrome2 {

	class Window : public QObject, public SceneIndicator {
	Q_OBJECT
	protected:
		bool shouldRegen = true;
		bool inversed = false;
	public:

		typedef enum {
			IntDynamic,
			IntStatic
		} Type;

		virtual Type type() = 0;

		virtual void mvHorizontal(int v) = 0;

		virtual void mvVertical(int v) = 0;

		virtual const Pixel &getLUT(quint64 value) = 0;

		virtual bool genLUT() = 0;

		inline bool isInversed() const {
			return inversed;
		}

		void setInversed(bool inversed) {
			if (Window::inversed == inversed) return;
			Window::inversed = inversed;
			shouldRegen = true;
		}
	};
}