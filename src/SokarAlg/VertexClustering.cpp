//
// Created by adam on 13.05.2021.
//

#include "./VertexClustering.hpp"
#include "Array3.hpp"

using namespace SokarAlg;

VertexClustering::VertexClustering(
		const glm::vec3 &clusterSize,
		const glm::vec3 &clusterOffset
) : clusterSize(clusterSize),
	clusterOffset(clusterOffset) {
}

IndexedMesh *VertexClustering::exec(const IndexedMesh *mesh) {

	auto extrema = findExtrema(mesh);

	auto newMesh = new IndexedMesh();

//	Array3<std::vector<qsizetype>> buckets(extrema.max.x, extrema.max.y, extrema.max.z);
//	std::vector<qsizetype> vertexMap(mesh->verticesCount());
//
//	for (auto iter = mesh->getVertices().begin(); iter != mesh->getVertices().end(); ++iter) {
//		auto index = iter - mesh->getVertices().begin();
//
//		auto clusterFI = (*iter - clusterOffset) / clusterSize;
//		auto clusterI = glm::i32vec3({
//											 std::floor(clusterFI.x),
//											 std::floor(clusterFI.y),
//											 std::floor(clusterFI.z)
//									 });
//
//		buckets(clusterI.x, clusterI.y, clusterI.z).push_back(index);
//	}
//
//	for (auto iter = mesh->getVertices().begin(); iter != mesh->getVertices().end(); ++iter) {
//	}

//	auto extrema = findExtrema(mesh);
//
//	std::map<glm::u32vec3, std::vector<qsizetype>> clusterMap;
//
//	// iterujemy po werzchołkach
//
//	for (auto iter = mesh->getVertices().begin(); iter != mesh->getVertices().end(); ++iter) {
////			iter->pos
//	}
//
//	// wyliczmy nowe wirzchołki
//


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
