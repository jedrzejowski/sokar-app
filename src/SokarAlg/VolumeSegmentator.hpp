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
	public:
		enum Compare {
			LessThen,
			MoreThen,
		};
	protected:

		QSharedPointer<const Volume> volume;
		QSharedPointer<MeshType> mesh = nullptr;
		float isoLevel = 0.5f;
		Compare isoCompare = LessThen;

		virtual void execAlg() = 0;

		virtual void execBefore();

	public:
		QFuture<void> execAsync();
		void execSync();

		[[nodiscard]]
		float getIsoLevel() const;
		void setIsoLevel(float isoLevel);

		Compare getIsoCompare() const;
		void setIsoCompare(Compare isoCompare);

		[[nodiscard]]
		const QSharedPointer<const Volume> &getVolume() const;
		void setVolume(const QSharedPointer<const Volume> &virtualVolume);

		void addTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2);

		[[nodiscard]]
		const QSharedPointer<MeshType> &getMesh() const;
	};
}



