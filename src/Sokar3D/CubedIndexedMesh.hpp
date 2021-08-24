//
// Created by adam on 23.08.2021.
//

#pragma once

#include "Sokar3D.hpp"
#include "Sokar3D/Mesh.hpp"

namespace Sokar3D {

    class CubedIndexedMesh : public Sokar3D::Mesh, public QEnableSharedFromThis<CubedIndexedMesh> {
    public:

        struct Index {
            glm::i32vec3 cube;
            glm::i32 vert;

            inline bool operator==(const Index &other) const {

                return cube == other.cube and vert == other.vert;
            }
        };

        struct Cube {
            glm::i32vec3 index;
            QVector<glm::vec3> vertices;
        };

        struct Face {
            Index i1, i2, i3;

            inline bool isDummy() const {

                return i1 == i2 or i2 == i3 or i1 == i3;
            }

            inline bool operator==(const Face &other) const {

                return (i1 == other.i1 and i2 == other.i2 and i3 == other.i3)
                       or
                       (i1 == other.i2 and i2 == other.i3 and i3 == other.i1)
                       or
                       (i1 == other.i3 and i2 == other.i1 and i3 == other.i2);
            }
        };

    private:
        QHash<qint32, QHash<qint32, QHash<qint32, Cube>>> vertices;
        QVector<Face> faces;
        glm::vec3 cubeSize = glm::vec3(1.f);

        CubedIndexedMesh();

    public:

        static CubedIndexedMeshPtr New();

        const glm::vec3 &getCubeSize() const;

        void setCubeSize(const glm::vec3 &cubeSize);

        void addTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2) override;

        void addTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, bool checkDup);

        Index addVertex(const glm::vec3 &v, bool checkDup = true);

        glm::i32vec3 position2cubeIndex(const glm::vec3 &v) const;

        TriangleListMeshPtr toTriangleListMesh() const override;
    };

}

