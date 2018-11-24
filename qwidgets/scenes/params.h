#pragma once

#include <QTransform>
#include <gdcmImageReader.h>
#include "_classdef.h"

struct Sokar::SceneParams {
	int frame = 0;
	const gdcm::ImageReader *imageReader;
	std::vector<char> *imageBuffer;
};
