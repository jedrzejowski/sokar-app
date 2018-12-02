#pragma once

#include <gdcmImageReader.h>

#include <QtCore>

#include "../_classdef.h"

namespace Sokar {
	struct SceneParams {
		ushort frame = 0;
		quint64 imgSize = 0;
		DicomSceneSet *dicomSceneSet;
		const gdcm::ImageReader *imageReader;
		std::vector<char> *imageBuffer;
	};
}
