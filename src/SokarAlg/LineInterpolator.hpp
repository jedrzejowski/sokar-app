//
// Created by adam on 19.09.2021.
//

#pragma once

#include "SokarAlg.hpp"
#include "Volume.hpp"

namespace SokarAlg {

    class LineInterpolator : public SokarLib::Displayable {
    protected:
        VolumeCPtr volume = nullptr;

        virtual void dicomVolumeChanged();

    public:
        virtual ~VolumeInterpolator();

        [[nodiscard]]
        const VolumeCPtr &getVolume() const;
        void setVolume(const VolumeCPtr &vv);

        [[nodiscard]]
        virtual glm::vec3 interpolate(float desireValue, const Volume::Point &A, const Volume::Point &B) const = 0;
    };

}

