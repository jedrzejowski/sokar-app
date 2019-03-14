#pragma once


#include <QtCore>

#include "_classdef.h"
#include "sokar/dicombundle.h"

namespace Sokar {
	struct SceneParams {
		const DicomBundle *dicomBundle;
		DicomSceneSet *dicomSceneSet;

		ushort frame = 0;
		quint64 imgSize = 0;
		std::vector<char> *imageBuffer;

	};
}
