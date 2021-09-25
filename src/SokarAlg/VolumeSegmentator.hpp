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
    protected:

        LineInterpolatorPtr line_interpolator = nullptr;
        VolumeCPtr volume = nullptr;
        Sokar3D::MeshPtr mesh = nullptr;
        float iso_level = 0.f;

        void execBefore() override;

    public:
        [[nodiscard]]
        float getIsoLevel() const;
        void setIsoLevel(float isoLevel);

        const LineInterpolatorPtr &getLineInterpolator() const;
        void setLineInterpolator(const LineInterpolatorPtr &lineInterpolator);

        [[nodiscard]]
        const QSharedPointer<const Volume> &getVolume() const;
        void setVolume(const VolumeCPtr &virtualVolume);

        void addTriangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2);

        [[nodiscard]]
        const Sokar3D::MeshPtr &getMesh() const;
        void setMesh(const Sokar3D::MeshPtr &mesh);

        [[nodiscard]]
        Volume::Point getPoint(const glm::i32vec3 &pos) const;

        [[nodiscard]]
        glm::vec3 interpolatePoint(const Volume::Point &p1, const Volume::Point &p2);
    };
}



