//
// Created by adam on 13.05.2021.
//

#include "SokarGlm.hpp"
#include "./VertexClustering.hpp"
#include <QtMath>
#include <map>

using namespace SokarAlg;

VertexClustering::VertexClustering() {
}

VertexClusteringPtr VertexClustering::New() {
	return SokarAlg::VertexClusteringPtr(new VertexClustering);
}

IndexedMeshPtr VertexClustering::exec() {
	auto newMesh = IndexedMesh::New();

	auto extrema = findExtrema();
	auto old_vertices = mesh->getVertices();

	qDebug() << "KLASTERYZUJEMY" << clusterOffset << clusterSize;

	std::map<glm::i32vec3, QVector<IndexedMesh::Size>> buckets;
	auto vertex2vertex = QVector<IndexedMesh::Size>(mesh->getVertices().size(), -1);

	auto iter_end = old_vertices.end();
	auto iter_begin = old_vertices.begin();
	for (auto iter = iter_begin; iter != iter_end; ++iter) {
		auto index = iter - old_vertices.begin();
		auto &vertex = *iter;


		auto clusterFI = (vertex - clusterOffset) / clusterSize;


		auto clusterI = glm::i32vec3({
											 qFloor(clusterFI.x),
											 qFloor(clusterFI.y),
											 qFloor(clusterFI.z)
									 });

//		buckets.at(clusterI).push_back(index);
	}

	qDebug() << "num of buckets" << buckets.size();

	for (auto iter = buckets.begin(); iter != buckets.end(); ++iter) {
		auto[bucket_location, bucket_indexes]{*iter};

		qDebug() << "bucket" << bucket_location << bucket_indexes.size();

		auto final_vertex = glm::vec3(0.f, 0.f, 0.f);
		for (const auto &index : bucket_indexes) {
			final_vertex += old_vertices[index];
		}
		final_vertex /= bucket_indexes.size();

		auto newIndex = newMesh->addVertex(final_vertex, false);

		for (const auto &index : bucket_indexes) {
			vertex2vertex[index] = newIndex;
		}
	}

	for (const auto &face : mesh->getFaces()) {
		newMesh->addTriangle(face.i1, face.i2, face.i3, false);
	}

	return newMesh;
}

const glm::vec3 &VertexClustering::getClusterSize() const {
	return clusterSize;
}

void VertexClustering::setClusterSize(const glm::vec3 &newClusterSize) {
	clusterSize = newClusterSize;
}

const glm::vec3 &VertexClustering::getClusterOffset() const {
	return clusterOffset;
}

void VertexClustering::setClusterOffset(const glm::vec3 &newClusterOffset) {
	clusterOffset = newClusterOffset;
}

QString VertexClustering::toDisplay() {
	return QString("%1").arg(
			"klasteryzacja wierzchołków"
	);
}
