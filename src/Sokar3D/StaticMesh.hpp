//
// Created by adam on 29.03.2021.
//

#pragma once

#include <QString>
#include <QFuture>
#include "./Sokar3D.hpp"
#include "./MeshVertex.h"

namespace Sokar3D {


	class StaticMesh : public QObject {
	Q_OBJECT

	protected:
		QVector<MeshVertex> vertices;

	public:

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

	};
}