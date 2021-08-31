//
// Created by adam on 23.08.2021.
//

#include "CubedIndexedMesh.hpp"
#include "IndexedMesh.hpp"
#include "TriangleListMesh.hpp"

using namespace Sokar3D;
using Index = CubedIndexedMesh::Index;

CubedIndexedMesh::CubedIndexedMesh() {

    sokarTrace();
}

CubedIndexedMeshPtr CubedIndexedMesh::New() {

    return Sokar3D::CubedIndexedMeshPtr(new CubedIndexedMesh());
}

glm::i32vec3 CubedIndexedMesh::position2cubeIndex(const glm::vec3 &v) const {

    return v / cubeSize;
}


CubedIndexedMesh::Index CubedIndexedMesh::addVertex(const glm::vec3 &newVertex, bool checkDup) {

    Index index{};
    index.cube = position2cubeIndex(newVertex);

    // ilośc zmarnowanych godzin na tej linii = 5
    auto &cube = vert_space[index.cube];

    if (checkDup) {

        auto itr = std::find_if(cube.vertices.begin(), cube.vertices.end(), [&](const auto &vertex) {
            return SokarGlm::fastInDistance(vertex, newVertex);
        });

        if (itr != cube.vertices.end()) {
            index.vert = itr.key();
            return index;
        }
    }

    ++next_index;
    cube.vertices[next_index] = newVertex;
    index.vert = next_index;

    return index;
}

void CubedIndexedMesh::addTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2) {

    addTriangle(v0, v1, v2, true);
}

void CubedIndexedMesh::addTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, bool checkDups) {

    auto i0 = addVertex(v0, checkDups);
    auto i1 = addVertex(v1, checkDups);
    auto i2 = addVertex(v2, checkDups);

    auto newFace = Face{i0, i1, i2};

    if (newFace.isDummy()) {
        return;
    }

//    if (checkDups) {
//
//        auto iter = faces.begin();
//        while (iter != faces.end()) {
//
//            if (iter->operator==(newFace)) {
//
//                return;
//            }
//        }
//    }

    faces << newFace;
}

const glm::vec3 &CubedIndexedMesh::getCubeSize() const {

    return cubeSize;
}

void CubedIndexedMesh::setCubeSize(const glm::vec3 &cubeSize) {

    CubedIndexedMesh::cubeSize = cubeSize;
}

void CubedIndexedMesh::foreachFaces(const std::function<void(Mesh::Face)> &functor) const {

    for (const auto &face: faces) {
        functor({
                        vert_space[face.i0.cube].vertices[face.i0.vert],
                        vert_space[face.i1.cube].vertices[face.i1.vert],
                        vert_space[face.i2.cube].vertices[face.i2.vert]
                });
    }
}

void CubedIndexedMesh::dump2wavefront(SokarLib::WavefrontObjBuilder &builder) const {

    SokarLib::HashCubeSpace<QHash<size_type, SokarLib::WavefrontObjBuilder::size_type>> my_vertex_2_obj_vertex;

    vert_space.constForEach([&](const auto &cube_index, const auto &cube) {

        for (auto iter = cube.vertices.constBegin(); iter != cube.vertices.constEnd(); ++iter) {

            auto i = builder.addVertex(iter.value());
            my_vertex_2_obj_vertex[cube_index][iter.key()] = i;
        }
    });


    for (const auto &face: faces) {

        builder.addFaceV(
                my_vertex_2_obj_vertex[face.i0.cube][face.i0.vert],
                my_vertex_2_obj_vertex[face.i1.cube][face.i1.vert],
                my_vertex_2_obj_vertex[face.i2.cube][face.i2.vert]
        );
    }
}

void CubedIndexedMesh::injectTo(const IndexedMeshPtr &other, const glm::mat4 &transform) const {

    SokarLib::HashCubeSpace<QHash<size_type, SokarLib::WavefrontObjBuilder::size_type>> my_vertex_2_other_vertex;

    vert_space.constForEach([&](const auto &cube_index, const auto &cube) {
        for (auto iter = cube.vertices.constKeyValueBegin(); iter != cube.vertices.constKeyValueEnd(); ++iter) {
            auto vertex_index = other->addVertex(iter->second, false);
            my_vertex_2_other_vertex[cube_index][iter->first] = vertex_index;
        }
    });

    for (const auto &face: faces) {
        qDebug() << my_vertex_2_other_vertex[face.i0.cube][face.i0.vert]
                 << my_vertex_2_other_vertex[face.i1.cube][face.i1.vert]
                 << my_vertex_2_other_vertex[face.i2.cube][face.i2.vert];

        other->addTriangle(
                my_vertex_2_other_vertex[face.i0.cube][face.i0.vert],
                my_vertex_2_other_vertex[face.i1.cube][face.i1.vert],
                my_vertex_2_other_vertex[face.i2.cube][face.i2.vert],
                false
        );
    }
}

void CubedIndexedMesh::injectTo(const TriangleListMeshPtr &other, const glm::mat4 &transform) const {

    for (const auto &face: faces) {
        other->addTriangle(
                vert_space[face.i0.cube].vertices[face.i0.vert],
                vert_space[face.i1.cube].vertices[face.i1.vert],
                vert_space[face.i2.cube].vertices[face.i2.vert]
        );
    }
}

void CubedIndexedMesh::applyTransform(const glm::mat4 &transform) {

    vert_space.forEach([&](const auto &cube_index, auto &cube) {
        for (auto &vert: cube.vertices) {
            vert = transform * glm::vec4(vert, 1.0f);
        }
    });
}

bool CubedIndexedMesh::Face::operator==(const CubedIndexedMesh::Face &other) const {

    return (i0 == other.i0 and i1 == other.i1 and i2 == other.i2)
           or
           (i0 == other.i1 and i1 == other.i2 and i2 == other.i0)
           or
           (i0 == other.i2 and i1 == other.i0 and i2 == other.i1);
}

bool CubedIndexedMesh::Face::isDummy() const {

    return i0 == i1 or i1 == i2 or i0 == i2;
}

bool CubedIndexedMesh::Index::operator==(const Index &other) const {

    return cube == other.cube and vert == other.vert;
}
