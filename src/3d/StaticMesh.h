//
// Created by adam on 29.03.2021.
//

#pragma once

#include <QString>
#include <QFuture>
#include "./_def.h"
#include "./MeshVertex.h"

namespace Sokar3D {


	class StaticMesh : public QObject {
	Q_OBJECT

	protected:
		QVector<MeshVertex> vertices;

	public:

		virtual void addTriangle(const MeshVertex &v0, const MeshVertex &v1, const MeshVertex &v2);

		static StaticMesh *createCubeMesh();

		[[nodiscard]]
		qsizetype verticesSizeInBytes() const;

		[[nodiscard]]
		qsizetype verticesCount() const;

		[[nodiscard]]
		const quint8 *verticesData() const;

		[[nodiscard]]
		const QVector<MeshVertex> &getVertices() const;

		[[nodiscard]]
		virtual StaticMesh *toStaticMash() const;

		[[nodiscard]]
		virtual IndexedStaticMesh *toIndexedStaticMesh() const;
	};

	class IndexedStaticMesh : public StaticMesh {
	Q_OBJECT

	protected:
		QVector<quint32> indexes;

	public:
		void addTriangle(const MeshVertex &v0, const MeshVertex &v1, const MeshVertex &v2) override;
		[[nodiscard]]
		qsizetype indexesSizeInBytes() const;

		[[nodiscard]]
		const quint8 *indexData() const;

		[[nodiscard]]
		qsizetype indexCount() const;

		[[nodiscard]]
		StaticMesh *toStaticMash() const override;

		[[nodiscard]]
		const QVector<quint32> &getIndexes() const;

		[[nodiscard]]
		IndexedStaticMesh *toIndexedStaticMesh() const override;

		[[nodiscard]]
		quint32 addVertex(const MeshVertex &v);
	};
}