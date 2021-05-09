//
// Created by adam on 14.03.2021.
//

#pragma once

#include "./_def.h"
#include "../_classdef.h"
#include "./VirtualVolume.h"
#include "./VertexInterpolator.h"
#include "../dicomview/scenes/sets/_sceneset.h"

namespace SokarAlg {

	class DicomVolume : public VirtualVolume {
	Q_OBJECT

		const Sokar::DicomSceneSet *sceneSet = nullptr;
		Sokar::DataConverter dataConverter;

		VertexInterpolator *interpolator = new LinearVertexInterpolator();

		glm::vec3 size;
		glm::u32vec3 trueSize;
		glm::vec3 cubeSize;

	public:
		~DicomVolume() override;

		[[nodiscard]]
		const Sokar::DicomSceneSet *getSceneSet() const;

		void setSceneSet(const Sokar::DicomSceneSet *sceneSet);

		[[nodiscard]]
		glm::vec3 getSize() const override;

		[[nodiscard]]
		glm::u32vec3 getTrueSize() const;

		[[nodiscard]]
		float getTrueValue(const glm::i32vec3 &position) const;

		[[nodiscard]]
		float getValue(const glm::vec3 &position) const override;

		[[nodiscard]]
		const glm::vec3 &getCubeSize() const;

		[[nodiscard]]
		VertexInterpolator *getInterpolator() const;
		void setInterpolator(VertexInterpolator *interpolator);

	private:

		void updateModel();
	};
}



