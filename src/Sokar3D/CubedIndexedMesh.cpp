//
// Created by adam on 23.08.2021.
//

#include "CubedIndexedMesh.hpp"
#include "TriangleListMesh.hpp"

using namespace Sokar3D;
using Index = CubedIndexedMesh::Index;

CubedIndexedMesh::CubedIndexedMesh() {}

CubedIndexedMeshPtr CubedIndexedMesh::New() {

    return Sokar3D::CubedIndexedMeshPtr(new CubedIndexedMesh());
}

glm::i32vec3 CubedIndexedMesh::position2cubeIndex(const glm::vec3 &v) const {

    return v / cubeSize;
}


CubedIndexedMesh::Index CubedIndexedMesh::addVertex(const glm::vec3 &newVertex, bool checkDup) {

    Index index{};
    index.cube = position2cubeIndex(newVertex);

    auto cube = vertices[index.cube.x][index.cube.y][index.cube.z];

    if (checkDup) {

        auto itr = std::find_if(cube.vertices.begin(), cube.vertices.end(), [&](const auto &vertex) {
            return SokarGlm::fastInDistance(vertex, newVertex);
        });

        if (itr != cube.vertices.end()) {
            index.vert = itr - cube.vertices.begin();
            return index;
        }
    }

    cube.vertices << newVertex;
    index.vert = cube.vertices.size() - 1;

    return index;
}

void CubedIndexedMesh::addTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2) {

    addTriangle(v0, v1, v2, true);
}

void CubedIndexedMesh::addTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, bool checkDups) {

    auto i0 = addVertex(v0);
    auto i1 = addVertex(v1);
    auto i2 = addVertex(v2);

    auto newFace = Face{i0, i1, i2};

    if (newFace.isDummy()) {
        return;
    }

    if (checkDups) {

        auto iter = faces.begin();
        while (iter != faces.end()) {

            if (iter->operator==(newFace)) {

                return;
            }
        }
    }

    faces << newFace;
}

const glm::vec3 &CubedIndexedMesh::getCubeSize() const {

    return cubeSize;
}

void CubedIndexedMesh::setCubeSize(const glm::vec3 &cubeSize) {

    CubedIndexedMesh::cubeSize = cubeSize;
}

void CubedIndexedMesh::foreachFaces(const std::function<void(Mesh::Face)> &functor) {

    for (const auto &face : faces) {
        functor({
                        vertices[face.i0.cube.x][face.i0.cube.y][face.i0.cube.z].vertices[face.i0.vert],
                        vertices[face.i1.cube.x][face.i1.cube.y][face.i1.cube.z].vertices[face.i1.vert],
                        vertices[face.i2.cube.x][face.i2.cube.y][face.i2.cube.z].vertices[face.i2.vert]
                });
    }
}

CubedIndexedMeshPtr CubedIndexedMesh::from(const MeshPtr &mesh) {

    auto new_mesh = New();

    mesh->foreachFaces([&](auto face) {
        new_mesh->addTriangle(face.v0, face.v1, face.v2);
    });

    return new_mesh;
}
