//
// Created by adam on 23.08.2021.
//

#pragma once

#include "Sokar3D.hpp"
#include "Sokar3D/Mesh.hpp"
#include "SokarLib/HashCubeSpace.hpp"

namespace Sokar3D {

    class CubedIndexedMesh : public Mesh, public QEnableSharedFromThis<CubedIndexedMesh> {
    public:
        using size_type = qint32;

        struct Index {
            glm::i32vec3 cube;
            size_type vert;

            bool operator==(const Index &other) const;
        };

        struct Cube {
            // glm::i32vec3 index;
            QHash<size_type, glm::vec3> vertices;
        };

        struct Face {
            Index i0, i1, i2;

            [[nodiscard]]
            inline bool isDummy() const;

            bool operator==(const Face &other) const;
        };

    private:
        size_type next_index = 0;
        SokarLib::HashCubeSpace<Cube> vert_space;
        QVector<Face> faces;
        glm::vec3 cubeSize = glm::vec3(1.f);

        CubedIndexedMesh();

    public:

        static CubedIndexedMeshPtr New();

        void foreachFaces(const std::function<void(Mesh::Face)> &functor) const override;

        const glm::vec3 &getCubeSize() const;

        void setCubeSize(const glm::vec3 &cubeSize);

        void addTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2) override;

        void addTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, bool checkDup);

        Index addVertex(const glm::vec3 &v, bool checkDup = true);

        glm::i32vec3 position2cubeIndex(const glm::vec3 &v) const;

        void dump2wavefront(SokarLib::WavefrontObjBuilder &builder) const override;

        void injectTo(const IndexedMeshPtr &other, const glm::mat4 &transform) const override;
        void injectTo(const TriangleListMeshPtr &other, const glm::mat4 &transform) const override;

        void applyTransform(const glm::mat4 &transform) override;
    };

}

