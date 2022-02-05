//
// Created by adam on 14.03.2021.
//

#pragma once

#include "SokarAlg.hpp"
#include "Volume.hpp"
#include "VolumeInterpolator.hpp"
#include "InterpolatedVolume.hpp"
#include "dicomview/scenes/sets/_sceneset.h"

namespace SokarAlg {

    class RawDicomVolume : public Volume {

        const Sokar::DicomSceneSet *sceneSet = nullptr;
        SokarDicom::DataConverter dataConverter;

    public:

        [[nodiscard]]
        const Sokar::DicomSceneSet *getSceneSet() const;

        void setSceneSet(const Sokar::DicomSceneSet *sceneSet);

        [[nodiscard]]
        virtual glm::vec3 getWokselSize() const;

        [[nodiscard]]
        glm::i32vec3 getSize() const override;

        [[nodiscard]]
        float getValue(const glm::i32vec3 &position) const override;
    };

    class DicomVolume : public InterpolatedVolume {
        QSharedPointer<const RawDicomVolume> rawDicomVolume;

        float cubesPerMM = 1.f;
        glm::vec3 wokselSize = glm::vec3(1.f);

    public:

        [[nodiscard]]
        const QSharedPointer<const RawDicomVolume> &getRawDicomVolume() const;
        void setRawDicomVolume(const QSharedPointer<const RawDicomVolume> &rawDicomVolume);

        [[nodiscard]]
        glm::i32vec3 getSize() const override;

        [[nodiscard]]
        float getCubesPerMM() const;
        void setCubesPerMM(float cubesPerMm);

    private:
        void update();
    };
}



