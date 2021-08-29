//
// Created by adam on 29.03.2021.
//

#pragma once

#include <QString>
#include <QFuture>
#include "Sokar3D.hpp"
#include "Sokar3D/Mesh.hpp"
#include <boundingmesh/Mesh.h>

namespace Sokar3D {


    class IndexedMesh : public Mesh, public QEnableSharedFromThis<IndexedMesh> {
    public:
        using Vertex = glm::vec3;
        using Size = int;

        struct Face {
            Size i0, i1, i2;

            [[nodiscard]]
            inline bool isDummy() const {

                return i0 == i1 or i1 == i2 or i0 == i2;
            }

            inline bool operator==(const Face &other) const {

                return (i0 == other.i0 and i1 == other.i1 and i2 == other.i2)
                       or
                       (i0 == other.i1 and i1 == other.i2 and i2 == other.i0)
                       or
                       (i0 == other.i2 and i1 == other.i0 and i2 == other.i1);
            }
        };

    protected:
        QVector<Vertex> vertices;
        QVector<Face> faces;

        IndexedMesh();

    public:

        [[nodiscard]]
        static IndexedMeshPtr New();

        [[nodiscard]]
        static IndexedMeshPtr from(const Sokar3D::MeshPtr &mesh, bool checkDuplicates = true);

        void foreachFaces(const std::function<void(Mesh::Face)> &functor) override;

        [[nodiscard]]
        Size verticesSizeInBytes() const;

        [[nodiscard]]
        Size verticesCount() const;

        [[nodiscard]]
        const quint8 *verticesData() const;

        [[nodiscard]]
        const QVector<Vertex> &getVertices() const;

        [[nodiscard]]
        int facesSizeInBytes() const;

        [[nodiscard]]
        const quint8 *indexData() const;

        [[nodiscard]]
        Size faceCount() const;

        [[nodiscard]]
        const QVector<Face> &getFaces() const;

        Size addVertex(const Vertex &v, bool checkDup = true);
        void addTriangle(const Vertex &v0, const Vertex &v1, const Vertex &v2) override;
        void addTriangle(const Vertex &v0, const Vertex &v1, const Vertex &v2, bool checkDup);
        void addTriangle(Size i0, Size i1, Size i2, bool checkDuplicates = true);

        boundingmesh::MeshPtr toBoundingMesh();
    };
}