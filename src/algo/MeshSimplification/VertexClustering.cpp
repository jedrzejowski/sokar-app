//
// Created by adam on 13.05.2021.
//

#include "./VertexClustering.h"

using namespace SokarAlg;

VertexClustering::VertexClustering(
		const glm::vec3 &clusterSize,
		const glm::vec3 &clusterOffset
) : clusterSize(clusterSize),
	clusterOffset(clusterOffset) {
}

IndexedMesh *VertexClustering::exec(const IndexedMesh *mesh) {

	auto newMesh = new IndexedMesh();

	std::map<glm::i32vec3, std::vector<qsizetype>> buckets;
	std::map<qsizetype, qsizetype> vertexMap;

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
