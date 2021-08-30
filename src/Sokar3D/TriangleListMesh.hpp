//
// Created by adam on 29.03.2021.
//

#pragma once

#include <QString>
#include <QFuture>
#include "./Sokar3D.hpp"
#include "./Mesh.hpp"
#include "./MeshVertex.hpp"

namespace Sokar3D {

    class TriangleListMesh : public Mesh, public QEnableSharedFromThis<TriangleListMesh> {
    public:
        using Vertex = MeshVertex;
        using Size = int;

        struct Face {
            Vertex v0, v1, v2;
        };

    protected:
        QVector<Face> faces;

        TriangleListMesh();
        TriangleListMesh(const TriangleListMesh &mesh) = delete;

    public:

        static TriangleListMeshPtr New();

        void addTriangle(const MeshVertex &v0, const MeshVertex &v1, const MeshVertex &v2);
        void addTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2) override;

        void foreachFaces(const std::function<void(Mesh::Face face)> &functor) const override;

        static TriangleListMesh *createCubeMesh();

        [[nodiscard]]
        Size verticesSizeInBytes() const;

        [[nodiscard]]
        Size verticesCount() const;

        [[nodiscard]]
        Size facesCount() const;

        [[nodiscard]]
        const quint8 *verticesData() const;

        [[nodiscard]]
        const QVector<Face> &getFaces() const;

        void dump2wavefront(SokarLib::WavefrontObjBuilder &builder) const override;

        static TriangleListMeshPtr from(const MeshPtr &mesh);
    };
}