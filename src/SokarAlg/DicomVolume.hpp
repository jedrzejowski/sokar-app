//
// Created by adam on 14.03.2021.
//

#pragma once

#include "SokarAlg.hpp"
#include "Volume.hpp"
#include "VolumeInterpolator.hpp"
#include "InterpolatedVolume.hpp"
#include "dicomview/scenes/sets/_sceneset.h"

namespace SokarAlg {

	class RawDicomVolume : public Volume {

		const Sokar::DicomSceneSet *sceneSet = nullptr;
		Sokar::DataConverter dataConverter;

		glm::i32vec3 size;
		glm::vec3 wokselSize;

	public:

		[[nodiscard]]
		const Sokar::DicomSceneSet *getSceneSet() const;

		void setSceneSet(const Sokar::DicomSceneSet *sceneSet);

		[[nodiscard]]
		const glm::vec3 &getWokselSize() const;


	private:

		void updateModel();
	};

	class DicomVolume : public InterpolatedVolume {
		QSharedPointer<const RawDicomVolume> rawDicomVolume = nullptr;

		float cubesPerMM = 1.f;

	public:

		[[nodiscard]]
		const QSharedPointer<const RawDicomVolume> &getRawDicomVolume() const;
		void setRawDicomVolume(const QSharedPointer<const RawDicomVolume> &rawDicomVolume);

		[[nodiscard]]
		glm::i32vec3 getSize() const override;

		[[nodiscard]]
		float getCubesPerMM() const;
		void setCubesPerMM(float cubesPerMm);

	private:
		void update();
	};
}



