//
// Created by adam on 29.03.2021.
//

#pragma once

#include <QString>
#include <QFuture>
#include <SokarLib/WavefrontObjBuilder.hpp>
#include "./Sokar3D.hpp"
#include "./MeshVertex.hpp"

namespace Sokar3D {

	class StaticMesh : public QEnableSharedFromThis<StaticMesh> {

	protected:
		QVector<MeshVertex> vertices;

		StaticMesh();

	public:

		static StaticMeshPtr New();

		void addTriangle(const MeshVertex &v0, const MeshVertex &v1, const MeshVertex &v2);
		void addTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2);

		static StaticMesh *createCubeMesh();

		[[nodiscard]]
		qsizetype verticesSizeInBytes() const;

		[[nodiscard]]
		qsizetype verticesCount() const;

		[[nodiscard]]
		const quint8 *verticesData() const;

		[[nodiscard]]
		const QVector<MeshVertex> &getVertices() const;

		void dump2wavefront(SokarLib::WavefrontObjBuilder& builder) const;
	};
}