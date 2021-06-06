//
// Created by adam on 03.05.2021.
//

#pragma once

#include "./SokarAlg.hpp"
#include "Volume.hpp"
#include "Sokar3D/StaticMesh.hpp"

namespace SokarAlg {

	class VolumeSegmentator {
//		using MeshType = IndexedMesh;
		using MeshType = Sokar3D::StaticMesh;
	protected:


		QSharedPointer<const Volume> volume;
		QSharedPointer<MeshType> mesh = nullptr;

		virtual void execAlg() = 0;

		virtual void execBefore();

	public:
		QFuture<void> execAsync();
		void execSync();

		[[nodiscard]]
		const QSharedPointer<const Volume> &getVolume() const;

		void setVolume(const QSharedPointer<const Volume> &virtualVolume);

		void addTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2);

		[[nodiscard]]
		const QSharedPointer<MeshType> &getMesh() const;
	};
}



