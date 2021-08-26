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
    qDebug() << "KLASTERYZUJEMY" << clusterOffset << clusterSize;

    struct ClusterVertex {
        int oldIndex;
        glm::vec3 position;
    };

    struct ClusterPiece {
        QVector<ClusterVertex> vertices;
    };

    auto newMesh = Sokar3D::IndexedMesh::New();

    auto extrema = findExtrema();
    auto old_vertices = mesh->getVertices();


    SokarLib::HashCubeSpace<ClusterPiece> cluster;
    auto vertex2vertex = QVector<Sokar3D::IndexedMesh::Size>(mesh->getVertices().size(), -1);

    auto iter_end = old_vertices.end();
    auto iter_begin = old_vertices.begin();
    qDebug() << "1";
    for (auto iter = iter_begin; iter != iter_end; ++iter) {
        int old_index = iter - old_vertices.begin();
        auto &vertex = *iter;

        auto cluster_index = position2clusterIndex(vertex);

        auto piece = cluster[cluster_index];

        piece.vertices << ClusterVertex{old_index, vertex};
    }
    qDebug() << "2";
    cluster.forEach([&](const glm::i32vec3 &index, const ClusterPiece &piece) {

        auto final_vertex = glm::vec3(0.f, 0.f, 0.f);
        for (const auto &vertex: piece.vertices) {
            final_vertex += vertex.position;
        }
        final_vertex /= piece.vertices.size();

        auto newIndex = newMesh->addVertex(final_vertex, false);

        for (const auto &vertex: piece.vertices) {
            vertex2vertex[vertex.oldIndex] = newIndex;
        }
    });

    for (const auto &face: mesh->getFaces()) {
        newMesh->addTriangle(vertex2vertex[face.i0], vertex2vertex[face.i1], vertex2vertex[face.i2], false);
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

glm::i32vec3 VertexClustering::position2clusterIndex(const glm::vec3 &v) const {

    return (v + clusterOffset) / clusterSize;
}


QString VertexClustering::toDisplay() {

    return QString("%1").arg(
            "klasteryzacja wierzchołków"
    );
}
