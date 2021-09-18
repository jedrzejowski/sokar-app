//
// Created by adam on 05.05.2021.
//

#pragma once

#include "SokarAlg.hpp"
#include "Volume.hpp"

namespace SokarAlg {

    class VolumeEnv : public VolumeDecorator {

        glm::i32vec3 env_size;
        float env_value;

        explicit VolumeEnv();

    public:

        [[nodiscard]]
        static VolumeEnvPtr New();


        [[nodiscard]]
        const glm::i32vec3 &getEnvSize() const;
        void setEnvSize(const glm::i32vec3 &env_size);
        void setEnvSize(float env_size);

        [[nodiscard]]
        float getEnvValue() const;
        void setEnvValue(float env_value);

        [[nodiscard]]
        glm::i32vec3 getSize() const override;

        [[nodiscard]]
        float getValue(const glm::i32vec3 &position) const override;
    };
}

