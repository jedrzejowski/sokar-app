//
// Created by adam on 13.05.2021.
//

#include "SokarGlm.hpp"
#include "./VertexClustering.hpp"
#include "SokarLib/HashCubeSpace.hpp"
#include <QtMath>
#include <map>

using namespace SokarAlg;

VertexClustering::VertexClustering() {
}

VertexClusteringPtr VertexClustering::New() {

    return SokarAlg::VertexClusteringPtr(new VertexClustering);
}

Sokar3D::IndexedMeshPtr VertexClustering::exec() {

    struct ClusterVertex {
        int oldIndex;
        glm::vec3 position;
    };

    struct ClusterPiece {
        QVector<ClusterVertex> vertices;
    };

    auto new_mesh = Sokar3D::IndexedMesh::New();

    auto extrema = findExtrema();
    auto old_vertices = mesh->getVertices();


    SokarLib::HashCubeSpace<ClusterPiece> cluster;
    auto vertex2vertex = QVector<Sokar3D::IndexedMesh::Size>(mesh->getVertices().size(), -1);

    auto iter_end = old_vertices.end();
    auto iter_begin = old_vertices.begin();

    for (auto iter = iter_begin; iter != iter_end; ++iter) {
        int old_index = iter - old_vertices.begin();
        auto &vertex = *iter;

        auto cluster_index = position2clusterIndex(vertex);

        auto& piece = cluster[cluster_index];

        piece.vertices << ClusterVertex{old_index, vertex};
    }

    cluster.forEach([&](const glm::i32vec3 &index, const ClusterPiece &piece) {

        auto final_vertex = glm::vec3(0.f, 0.f, 0.f);
        for (const auto &vertex: piece.vertices) {
            final_vertex += vertex.position;
        }
        final_vertex /= piece.vertices.size();

        auto newIndex = new_mesh->addVertex(final_vertex, false);

        for (const auto &vertex: piece.vertices) {
            vertex2vertex[vertex.oldIndex] = newIndex;
        }
    });

    for (const auto &face: mesh->getFaces()) {
        new_mesh->addTriangle(vertex2vertex[face.i0], vertex2vertex[face.i1], vertex2vertex[face.i2], false);
    }

    return new_mesh;
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

glm::i32vec3 VertexClustering::position2clusterIndex(const glm::vec3 &v) const {

    auto pos = (v + clusterOffset) / clusterSize;

    return {qFloor(pos.x), qFloor(pos.y), qFloor(pos.z)};
}


QString VertexClustering::toDisplay() {

    return QString("%1").arg(
            "klasteryzacja wierzchołków"
    );
}
