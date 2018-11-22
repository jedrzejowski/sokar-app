#pragma once

#include <QObject>

#include "sokar/pixel.h"
#include "../../indicators/_base.h"

namespace Sokar::Monochrome2 {
	class Window : public QObject, public SceneIndicator {
	Q_OBJECT

		virtual const Pixel &getLUT(__int128 value);dic

		virtual void genLUT() = 0;
	};
}