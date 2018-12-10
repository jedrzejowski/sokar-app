#pragma once

#include <QObject>

#include "sokar/pixel.h"
#include "sokar/palette.h"
#include "../../indicators/_base.h"

#define __int128Problem(x) x = ~x + 1;

namespace Sokar::Monochrome {

	//http://gcc.gnu.org/onlinedocs/gcc/_005f_005fint128.html
	//xD po całości, 8h na to zmarnowałem

	typedef __int128 TrueInt;
	typedef unsigned __int128 TrueUInt;

	class Window : public QObject, public SceneIndicator {
	Q_OBJECT
	protected:
		bool shouldRegen = true;
		bool inversed = false;

		Palette *palette = nullptr;

		QMenu toolbarMenu;

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

		inline bool isInversed() const { return inversed; }

		QMenu *getMenu();

		void setInversed(bool inversed);

		Palette *getPalette() const { return palette; }

		void setPalette(Palette *palette);

	protected:
		virtual void genMenu() = 0;

	signals:
		void forceRefreshSignal();
	};
}