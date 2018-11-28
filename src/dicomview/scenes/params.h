#pragma once

#include <gdcmImageReader.h>

#include "_classdef.h"

namespace Sokar {
	struct SceneParams {
		int frame = 0;
		const DicomSceneSet *dicomSceneSet;
		const gdcm::ImageReader *imageReader;
		std::vector<char> *imageBuffer;
	};
}
