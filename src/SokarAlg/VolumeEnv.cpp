//
// Created by adam on 05.05.2021.
//

#include "VolumeEnv.hpp"

using namespace SokarAlg;

VolumeEnv::VolumeEnv()
        : env_size(1, 1, 1), env_value(0.0f) {
}

VolumeEnvPtr VolumeEnv::New() {

    return VolumeEnvPtr(new VolumeEnv);
}

glm::i32vec3 VolumeEnv::getSize() const {

    return volume->getSize() + env_size * 2;
}

float VolumeEnv::getValue(const glm::i32vec3 &position) const {

    auto pos = position - env_size;

    if (volume->isInVolume(pos)) {
        return volume->getValue(pos);
    }

    return env_value;
}

const glm::i32vec3 &VolumeEnv::getEnvSize() const {

    return env_size;
}

void VolumeEnv::setEnvSize(const glm::i32vec3 &new_env_size) {

    env_size = new_env_size;
}

void VolumeEnv::setEnvSize(float new_env_size) {

    env_size = {new_env_size, new_env_size, new_env_size};
}

float VolumeEnv::getEnvValue() const {

    return env_value;
}

void VolumeEnv::setEnvValue(float new_env_value) {

    env_value = new_env_value;
}
