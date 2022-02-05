//
// Created by adam on 06.02.2022.
//

#pragma once

#include "SokarAlg.hpp"
#include "Volume.hpp"
#include "DicomVolume.hpp"

namespace SokarAlg {

    class DebugCubeVolume : public RawDicomVolume {

        RawDicomVolumePtr raw_dicom_volume;

        glm::i32vec3 size;
        glm::i32vec3 from;
        glm::i32vec3 to;

    private:
        explicit DebugCubeVolume() = default;

    public:

        static DebugCubeVolumePtr create(const RawDicomVolumePtr &raw_dicom_volume);

        [[nodiscard]]
        glm::i32vec3 getSize() const override;

        [[nodiscard]]
        float getValue(const glm::i32vec3 &position) const override;

        [[nodiscard]]
        glm::vec3 getWokselSize() const override;
    };

    class DebugSphereVolume : public RawDicomVolume {

        RawDicomVolumePtr raw_dicom_volume;

        glm::i32vec3 size;
        glm::vec3 center;
        float radius;

    private:
        explicit DebugSphereVolume() = default;

    public:

        static DebugSphereVolumePtr create(const RawDicomVolumePtr &raw_dicom_volume);

        [[nodiscard]]
        glm::i32vec3 getSize() const override;

        [[nodiscard]]
        float getValue(const glm::i32vec3 &position) const override;

        [[nodiscard]]
        glm::vec3 getWokselSize() const override;
    };

}

