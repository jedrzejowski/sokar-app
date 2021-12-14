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
        using size_type = int;

        struct Face {
            size_type i0, i1, i2;

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

    private:
        size_type next_vert_index = 0;
        QHash<size_type, Vertex> vertices;
        QVector<Face> faces;

        IndexedMesh();

    public:

        [[nodiscard]]
        const QHash<size_type, Vertex> &getVertices() const;

        [[nodiscard]]
        const QVector<Face> &getFaces() const;

        [[nodiscard]]
        static IndexedMeshPtr New();

        void foreachFaces(const std::function<void(Mesh::Face)> &functor) const override;

        size_type addVertex(const Vertex &v, bool checkDup = true);
        void addTriangle(const Vertex &v0, const Vertex &v1, const Vertex &v2) override;
        void addTriangle(const Vertex &v0, const Vertex &v1, const Vertex &v2, bool checkDup);
        void addTriangle(size_type i0, size_type i1, size_type i2, bool checkDuplicates = true);

        void dump2wavefront(SokarLib::WavefrontObjBuilder &builder) const override;

        void injectTo(const IndexedMeshPtr &other, const glm::mat4 &transform) const override;
        void injectTo(const TriangleListMeshPtr &other, const glm::mat4 &transform) const override;

        void applyTransform(const glm::mat4 &transform) override;
    };
}