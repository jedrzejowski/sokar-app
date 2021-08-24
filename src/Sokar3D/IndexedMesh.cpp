//
// Created by adam on 29.03.2021.
//

#include "./IndexedMesh.hpp"
#include "Sokar3D/TriangleListMesh.hpp"
#include <QtConcurrent/QtConcurrentRun>
#include <QFile>

using namespace Sokar3D;
using Face = IndexedMesh::Face;
using Size = IndexedMesh::Size;

IndexedMesh::IndexedMesh() = default;

IndexedMeshPtr IndexedMesh::New() {

    return IndexedMeshPtr(new IndexedMesh);
}

//region getter & setters&

Size IndexedMesh::verticesSizeInBytes() const {

    return vertices.size() * static_cast<Size>(sizeof(Vertex));
}

Size IndexedMesh::verticesCount() const {

    return vertices.size();
}

const quint8 *IndexedMesh::verticesData() const {

    return reinterpret_cast<const quint8 *>(vertices.data());
}

const QVector<IndexedMesh::Vertex> &IndexedMesh::getVertices() const {

    return vertices;
}

int IndexedMesh::faceCount() const {

    return faces.size();
}

const QVector<Face> &IndexedMesh::getFaces() const {

    return faces;
}

const quint8 *IndexedMesh::indexData() const {

    return reinterpret_cast<const quint8 *>(faces.data());
}

Size IndexedMesh::facesSizeInBytes() const {

    return faces.size() * static_cast<Size>(sizeof(Face));
}

//endregion


Size IndexedMesh::addVertex(const Vertex &newVertex, bool checkDup) {

    if (checkDup) {

        auto itr = std::find_if(vertices.begin(), vertices.end(), [&](const auto &vertex) {
            return SokarGlm::fastInDistance(vertex, newVertex);
        });

        if (itr != vertices.end()) {
            return itr - vertices.begin();
        }
    }

    vertices << newVertex;
    return vertices.size() - 1;
}

void IndexedMesh::addTriangle(
        Size i0,
        Size i1,
        Size i2,
        bool checkDup
) {

    auto newFace = Face{i0, i1, i2};

    if (newFace.isDummy()) {
        return;
    }

    if (checkDup) {

        auto iter = faces.begin();
        while (iter != faces.end()) {

            if (iter->operator==(newFace)) {

                return;
            }
        }
    }

    faces << newFace;
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

void IndexedMesh::addTriangle(
        const IndexedMesh::Vertex &v0,
        const IndexedMesh::Vertex &v1,
        const IndexedMesh::Vertex &v2
) {

    addTriangle(v0, v1, v2, true);
}

void IndexedMesh::foreachFaces(const std::function<void(Mesh::Face)> &functor) {

    for (const auto &face : faces) {
        functor({vertices[face.i1], vertices[face.i2], vertices[face.i3]});
    }
}

IndexedMeshPtr IndexedMesh::from(const Sokar3D::MeshPtr &mesh) {

    auto indexedMesh = IndexedMesh::New();

    mesh->foreachFaces([&](auto face) {
        indexedMesh->addTriangle(face.v0, face.v1, face.v2);
    });

    return indexedMesh;
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
