//
// Created by adam on 03.05.2021.
//

#pragma once

#include "./SokarAlg.hpp"
#include "Volume.hpp"
#include "Sokar3D/StaticMesh.hpp"

namespace SokarAlg {

	class VolumeSegmentator {
	protected:

		const Volume *volume = nullptr;
		IndexedMesh *mesh = nullptr;

		virtual QFuture<void> execAlg() = 0;

	public:
		QFuture<void> exec();

		[[nodiscard]]
		const Volume *getVolume() const;

		void setVolume(const Volume *virtualVolume);

		void addTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2);

		[[nodiscard]]
		const IndexedMesh *getMesh() const;
	};
}



