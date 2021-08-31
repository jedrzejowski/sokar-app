//
// Created by adam on 29.03.2021.
//

#include "./IndexedMesh.hpp"
#include "Sokar3D/TriangleListMesh.hpp"
#include <QtConcurrent/QtConcurrentRun>
#include <QFile>

using namespace Sokar3D;
using Face = IndexedMesh::Face;
using size_type = IndexedMesh::size_type;

IndexedMesh::IndexedMesh() = default;

IndexedMeshPtr IndexedMesh::New() {

    return IndexedMeshPtr(new IndexedMesh);
}


size_type IndexedMesh::addVertex(const glm::vec3 &new_vertex, bool checkDup) {

    if (checkDup) {

        auto itr = std::find_if(vertices.begin(), vertices.end(), [&](const auto &vertex) {
            return SokarGlm::fastInDistance(vertex, new_vertex);
        });

        if (itr != vertices.end()) {
            return itr.key();
        }
    }
    ++next_vert_index;
    vertices[next_vert_index] = new_vertex;
    return next_vert_index;
}

void IndexedMesh::addTriangle(
        size_type i0,
        size_type i1,
        size_type i2,
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

boundingmesh::MeshPtr IndexedMesh::toBoundingMesh() const {

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

    auto vert_count = vertices.size();

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

void IndexedMesh::injectTo(const IndexedMeshPtr &other, const glm::mat4 &transform) const {

    QHash<size_type, size_type> my_index_to_other_index;

    for (auto itr = vertices.keyValueBegin(); itr != vertices.keyValueEnd(); ++itr) {

        my_index_to_other_index[itr->first] = other->addVertex(itr->second, false);
    }

    for (const auto &face: faces) {
        other->addTriangle(
                my_index_to_other_index[face.i0],
                my_index_to_other_index[face.i1],
                my_index_to_other_index[face.i2]
        );
    }
}

void IndexedMesh::injectTo(const TriangleListMeshPtr &other, const glm::mat4 &transform) const {

    for (const auto &face: faces) {
        other->addTriangle(
                vertices[face.i0],
                vertices[face.i1],
                vertices[face.i2]
        );
    }
}

const QHash<size_type, glm::vec3> &IndexedMesh::getVertices() const {

    return vertices;
}

const QVector<Face> &IndexedMesh::getFaces() const {

    return faces;
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
