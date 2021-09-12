//
// Created by adam on 03.05.2021.
//

#pragma once

#include "SokarLib/Displayable.hpp"
#include "./SokarAlg.hpp"
#include "./Volume.hpp"
#include "./Range.hpp"
#include "Sokar3D/Mesh.hpp"
#include "Algorithm.hpp"

namespace SokarAlg {

    class VolumeSegmentator : public Algorithm<Sokar3D::MeshPtr> {
    public:
        struct Point {
            glm::i32vec3 position;
            float value;
        };
    protected:

        VolumeInterpolatorPtr volumeInterpolator;
        VolumeCPtr volume;
        Sokar3D::MeshPtr mesh;
        Range<float> isoLevel = {0.5f, 1.f};

        void execBefore() override;

    public:
        [[nodiscard]]
        Range<float> getIsoLevel() const;
        void setIsoLevel(Range<float> isoLevel);

        [[nodiscard]]
        const VolumeInterpolatorPtr &getVolumeInterpolator() const;
        void setVolumeInterpolator(const VolumeInterpolatorPtr &volumeInterpolator);

        [[nodiscard]]
        const QSharedPointer<const Volume> &getVolume() const;
        void setVolume(const VolumeCPtr &virtualVolume);

        void addTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2);

        [[nodiscard]]
        const Sokar3D::MeshPtr &getMesh() const;
        void setMesh(const Sokar3D::MeshPtr &mesh);

        [[nodiscard]]
        Point getPoint(const glm::i32vec3 &pos) const;
    };
}



