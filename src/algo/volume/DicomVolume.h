//
// Created by adam on 14.03.2021.
//

#pragma once

#include "../_def.h"
#include "_classdef.h"
#include "./VirtualVolume.h"
#include "../ValueInterpolator.h"
#include "dicomview/scenes/sets/_sceneset.h"

namespace SokarAlg {

	class DicomVolume : public VirtualVolume {
	Q_OBJECT

		const Sokar::DicomSceneSet *sceneSet = nullptr;
		Sokar::DataConverter dataConverter;

		ValueInterpolator *interpolator = nullptr;

		glm::i32vec3 size;
		glm::i32vec3 trueSize;
		glm::vec3 wokselSize;
		float cubesPerMM = 1.f;

	public:
		~DicomVolume() override;

		[[nodiscard]]
		const Sokar::DicomSceneSet *getSceneSet() const;

		void setSceneSet(const Sokar::DicomSceneSet *sceneSet);

		[[nodiscard]]
		glm::i32vec3 getSize() const override;

		[[nodiscard]]
		glm::i32vec3 getTrueSize() const;

		[[nodiscard]]
		float getTrueValue(const glm::i32vec3 &position) const;

		[[nodiscard]]
		float getTrueValueSafe(const glm::i32vec3 &position) const;

		[[nodiscard]]
		float getValue(const glm::i32vec3 &position) const override;

		float getCubesPerMM() const;
		void setCubesPerMM(float cubesPerMm);

		[[nodiscard]]
		ValueInterpolator *getInterpolator() const;
		void setInterpolator(ValueInterpolator *interpolator);

	private:

		void updateModel();
	};
}



