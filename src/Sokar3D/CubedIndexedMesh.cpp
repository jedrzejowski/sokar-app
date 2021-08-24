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


TriangleListMeshPtr CubedIndexedMesh::toTriangleListMesh() const {

    auto tri_mesh = TriangleListMesh::New();

//    for (auto &x : vertices) {
//        x.
//    }

    return tri_mesh;
}

const glm::vec3 &CubedIndexedMesh::getCubeSize() const {

    return cubeSize;
}

void CubedIndexedMesh::setCubeSize(const glm::vec3 &cubeSize) {

    CubedIndexedMesh::cubeSize = cubeSize;
}
