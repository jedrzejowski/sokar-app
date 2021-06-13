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
	public:
		using Vertex = MeshVertex;
		using Size = int;

		struct Face {
			Vertex v1, v2, v3;
		};

	protected:
		QVector<Face> faces;

		StaticMesh();

	public:

		static StaticMeshPtr New();

		void addTriangle(const MeshVertex &v0, const MeshVertex &v1, const MeshVertex &v2);
		void addTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2);

		static StaticMesh *createCubeMesh();

		[[nodiscard]]
		Size verticesSizeInBytes() const;

		[[nodiscard]]
		Size verticesCount() const;

		[[nodiscard]]
		const quint8 *verticesData() const;

		[[nodiscard]]
		const QVector<Face> &getFaces() const;

		void dump2wavefront(SokarLib::WavefrontObjBuilder &builder) const;
	};
}