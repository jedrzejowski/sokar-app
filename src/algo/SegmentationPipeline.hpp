//
// Created by adam on 20.05.2021.
//

#pragma once

#include "_def.h"
#include "VolumeSegmentator.hpp"

namespace SokarAlg {

	class SegmentationPipeline : public QObject {
		DicomVolumeInterpolato* volumeIterpolator;
		VolumeSegmentator* volumeSegmentator;
		MeshSimplificator* meshSimplificator;
	public:
	};
}

