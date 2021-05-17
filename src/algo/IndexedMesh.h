//
// Created by adam on 29.03.2021.
//

#pragma once

#include <QString>
#include <QFuture>
#include "3d/StaticMesh.hpp"
#include "./_def.h"

namespace SokarAlg {


	class IndexedMesh : public QObject {
	Q_OBJECT
		using Vertex = glm::vec3;

	protected:
		std::vector<Vertex> vertices;
		std::vector<quint32> indexes;

	public:

		[[nodiscard]]
		qsizetype verticesSizeInBytes() const;

		[[nodiscard]]
		qsizetype verticesCount() const;

		[[nodiscard]]
		const quint8 *verticesData() const;

		[[nodiscard]]
		const std::vector<Vertex> &getVertices() const;

		[[nodiscard]]
		qsizetype indexesSizeInBytes() const;

		[[nodiscard]]
		const quint8 *indexData() const;

		[[nodiscard]]
		qsizetype indexCount() const;

		[[nodiscard]]
		const std::vector<quint32> &getIndexes() const;

		[[nodiscard]]
		quint32 addVertex(const Vertex &v, bool checkDup = true);

		void addTriangle(const Vertex &v0, const Vertex &v1, const Vertex &v2, bool checkDup = true);
		void addTriangle(quint32 i0, quint32 v1, quint32 v2, bool checkDuplicates = true);

		[[nodiscard]]
		Sokar3D::StaticMesh *toStaticMash() const;

	};
}