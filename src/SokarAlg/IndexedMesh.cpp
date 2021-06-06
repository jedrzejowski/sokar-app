//
// Created by adam on 29.03.2021.
//

#include "./IndexedMesh.hpp"
#include <QtConcurrent/QtConcurrentRun>
#include <QFile>

using namespace SokarAlg;

//region getter & setters&

qsizetype IndexedMesh::verticesSizeInBytes() const {
	return vertices.size() * sizeof(Vertex);
}

qsizetype IndexedMesh::verticesCount() const {
	return vertices.size();
}

const quint8 *IndexedMesh::verticesData() const {
	return reinterpret_cast<const quint8 *>(vertices.data());
}

const QVector<IndexedMesh::Vertex> &IndexedMesh::getVertices() const {
	return vertices;
}

qsizetype IndexedMesh::indexCount() const {
	return indexes.size();
}

const QVector<quint32> &IndexedMesh::getIndexes() const {
	return indexes;
}

const quint8 *IndexedMesh::indexData() const {
	return reinterpret_cast<const quint8 *>(indexes.data());
}

qsizetype IndexedMesh::indexesSizeInBytes() const {
	return indexes.size() * sizeof(quint32);
}

//endregion


quint32 IndexedMesh::addVertex(const Vertex &newVertex, bool checkDup) {

	if (checkDup) {

		auto itr = std::find_if(vertices.begin(), vertices.end(), [&](const auto &vertex) {
			return SokarGlm::fastInDistance(vertex, newVertex);
		});

		if (itr != vertices.end()) {
			return itr - vertices.begin();
		}
	}

	vertices.push_back(newVertex);
	return vertices.size() - 1;
}

void IndexedMesh::addTriangle(
		quint32 i0,
		quint32 i1,
		quint32 i2,
		bool checkDup
) {

	if (i0 == i1 || i1 == i2 || i0 == i2) {
		return;
	}

	if (checkDup) {

		auto iter = indexes.begin();
		while (iter != indexes.end()) {
			auto n0 = *iter++;
			auto n1 = *iter++;
			auto n2 = *iter++;

			if (n0 == i0 && n1 == i1 && n2 == i2 ||
				n0 == i1 && n1 == i2 && n2 == i0 ||
				n0 == i2 && n1 == i0 && n2 == i1) {

				return;
			}
		}
	}

	indexes.push_back(i0);
	indexes.push_back(i1);
	indexes.push_back(i2);
}


void IndexedMesh::addTriangle(
		const Vertex &v0,
		const Vertex &v1,
		const Vertex &v2,
		bool checkDup
) {

	auto i0 = addVertex(v0, checkDup);
	auto i1 = addVertex(v1, checkDup);
	auto i2 = addVertex(v2, checkDup);

	addTriangle(i0, i1, i2, checkDup);
}

QSharedPointer<Sokar3D::StaticMesh> IndexedMesh::toStaticMash() const {
	auto newMesh = new Sokar3D::StaticMesh();

	auto iter = indexes.begin();
	while (iter != indexes.end()) {
		newMesh->addTriangle(
				vertices[*iter++],
				vertices[*iter++],
				vertices[*iter++]
		);
	}

	return QSharedPointer<Sokar3D::StaticMesh>(newMesh);
}

//region Converters

//IndexedMesh *IndexedMesh::toStaticMash() const {
//	auto newMesh = new IndexedMesh();
//
//	newMesh->vertices = QVector<MeshVertex>(vertices);
//
//	return newMesh;
//}
//
//IndexedIndexedMesh *IndexedIndexedMesh::toIndexedIndexedMesh() const {
//	auto newMesh = new IndexedIndexedMesh();
//
//	newMesh->vertices = QVector<MeshVertex>(vertices);
//	newMesh->indexes = QVector<quint32>(indexes);
//
//	return newMesh;
//}
//
//IndexedMesh *IndexedIndexedMesh::toStaticMash() const {
//	auto newMesh = new IndexedMesh();
//
//
//	return newMesh;
//}
//
//IndexedIndexedMesh *IndexedMesh::toIndexedIndexedMesh() const {
//
//	auto newMesh = new IndexedIndexedMesh();
//
//
//	while (iter != vertices.end()) {
//		newMesh->addTriangle(*iter++, *iter++, *iter++);
//	}
//
//	return newMesh;
//}

//endregion
