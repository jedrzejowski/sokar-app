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

void IndexedMesh::foreachFaces(const std::function<void(Mesh::Face)> &functor) const {

    for (const auto &face: faces) {
        functor({vertices[face.i0], vertices[face.i1], vertices[face.i2]});
    }
}

IndexedMeshPtr IndexedMesh::from(const Sokar3D::MeshPtr &mesh, bool checkDuplicates) {

    auto indexedMesh = IndexedMesh::New();

    mesh->foreachFaces([&](auto face) {
        indexedMesh->addTriangle(face.v0, face.v1, face.v2, checkDuplicates);
    });

    return indexedMesh;
}

boundingmesh::MeshPtr IndexedMesh::toBoundingMesh() {

    auto new_mesh = std::make_shared<boundingmesh::Mesh>();

    // UWAGA: tu stosuje trik, ponieważ indeksy w boundingmesh::Mesh powinny wyjść takie same to można wyników nie zapisywać

    for (const auto &vert: vertices) {
        new_mesh->addVertex(boundingmesh::Vector3{vert.x, vert.y, vert.z});
    }

    for (const auto &face: faces) {
        new_mesh->addTriangle(face.i0, face.i1, face.i2);
    }

    return new_mesh;
}

void IndexedMesh::dump2wavefront(SokarLib::WavefrontObjBuilder &builder) const {

    auto vert_count = verticesCount();

    QVector<SokarLib::WavefrontObjBuilder::size_type> my_vertex_2_obj_vertex;
    my_vertex_2_obj_vertex.resize(vert_count);

    int i = -1;

    for (const auto &vert: vertices) {
        auto vi = builder.addVertex(vert);

        my_vertex_2_obj_vertex[i] = vi;
    }

    for (const auto &face: faces) {

        builder.addFaceV(
                my_vertex_2_obj_vertex[face.i0],
                my_vertex_2_obj_vertex[face.i1],
                my_vertex_2_obj_vertex[face.i2]
        );
    }
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
