//
// Created by adam on 20.05.2021.
//

#pragma once

#include "SokarAlg.hpp"
#include "VolumeSegmentator.hpp"
#include "DicomVolume.hpp"

namespace SokarAlg {

	class SegmentationPipeline : public QObject {
	public:

		QSharedPointer<const RawDicomVolume> rawDicomVolume;
		QSharedPointer<VolumeInterpolator> volumeInterpolator;
		QSharedPointer<VolumeSegmentator> volumeSegmentator;
		QSharedPointer<MeshSimplificator> meshSimplificator;

		QFuture<QSharedPointer<Sokar3D::StaticMesh>> executePipeline();
	};
}

