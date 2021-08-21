//
// Created by adam on 29.03.2021.
//

#pragma once

#include <QString>
#include <QFuture>
#include "Sokar3D.hpp"
#include "Sokar3D/Mesh.hpp"

namespace Sokar3D {


    class IndexedMesh : public Sokar3D::Mesh, public QEnableSharedFromThis<IndexedMesh> {
    public:
        using Vertex = glm::vec3;
        using Size = int;

        struct Face {
            Size i1, i2, i3;

            inline bool isDummy() const {

                return i1 == i2 || i2 == i3 || i1 == i3;
            }

            inline bool operator==(const Face &other) const {

                return i1 == other.i1 && i2 == other.i2 && i3 == other.i3 ||
                       i1 == other.i2 && i2 == other.i3 && i3 == other.i1 ||
                       i1 == other.i3 && i2 == other.i1 && i3 == other.i2;
            }
        };

    protected:
        QVector<Vertex> vertices;
        QVector<Face> faces;

        IndexedMesh();

    public:

        static IndexedMeshPtr New();

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
        void addTriangle(const Vertex &v0, const Vertex &v1, const Vertex &v2, bool checkDup = true);
        void addTriangle(Size i0, Size i1, Size i2, bool checkDuplicates = true);

        [[nodiscard]]
        Sokar3D::StaticMeshPtr toStaticMesh() const override;

        [[nodiscard]]
        static QFuture<IndexedMeshPtr> fromStaticMash(const Sokar3D::StaticMeshPtr &mesh);
    };
}