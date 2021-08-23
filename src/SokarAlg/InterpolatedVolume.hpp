//
// Created by adam on 20.05.2021.
//

#pragma once

#include "SokarAlg.hpp"
#include "Volume.hpp"

namespace SokarAlg {

    class InterpolatedVolume : public Volume {
        using SpaceTranslator = std::function<glm::vec3(const glm::vec3 &position)>;

        SpaceTranslator spaceTranslator;
        QSharedPointer<VolumeInterpolator> interpolator = nullptr;
        QSharedPointer<const Volume> volume = nullptr;
        glm::i32vec3 size;

    public:
        [[nodiscard]]
        const SpaceTranslator &getSpaceTranslator() const;
        void setSpaceTranslator(const SpaceTranslator &spaceTranslator);

        [[nodiscard]]
        const QSharedPointer<VolumeInterpolator> &getInterpolator() const;
        void setInterpolator(const QSharedPointer<VolumeInterpolator> &interpolator);

        [[nodiscard]]
        const QSharedPointer<const Volume> &getVolume() const;
        void setVolume(const QSharedPointer<const Volume> &volume);

        [[nodiscard]]
        glm::i32vec3 getSize() const override;
        void setSize(const glm::i32vec3 &size);

        [[nodiscard]]
        float getValue(const glm::i32vec3 &position) const override;

    private:
        void update();
    };
}



