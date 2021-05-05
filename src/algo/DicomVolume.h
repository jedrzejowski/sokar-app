//
// Created by adam on 14.03.2021.
//

#pragma once

#include "./_def.h"
#include "../_classdef.h"
#include "./VirtualVolume.h"
#include "../dicomview/scenes/sets/_sceneset.h"

namespace SokarAlg {

	class DicomVolume : public VirtualVolume {
	Q_OBJECT

		const Sokar::DicomSceneSet *sceneSet = nullptr;
		Sokar::DataConverter dataConverter;

		glm::u32vec3 size;
		float upScale = 1.f;
		glm::vec3 scale;
		glm::mat4 model;
		glm::mat4 inverseModel;

	public:

		[[nodiscard]]
		const Sokar::DicomSceneSet *getSceneSet() const;

		void setSceneSet(const Sokar::DicomSceneSet *sceneSet);

		[[nodiscard]]
		glm::u32vec3 getSize() const override;

		[[nodiscard]]
		float getValue(quint32 x, quint32 y, quint32 z) const override;

		[[nodiscard]]
		float getUpScale() const;
		void setUpScale(float upScale);

	private:

		void updateModel();
	};
}



