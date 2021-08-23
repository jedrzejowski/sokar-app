//
// Created by adam on 20.05.2021.
//

#include "CachedVolume.hpp"

using namespace SokarAlg;

void CachedVolume::volumeChanged() {

    cache.resize({0, 0, 0});
}

glm::i32vec3 CachedVolume::getSize() const {

    return size;
}

float CachedVolume::getValue(const glm::i32vec3 &position) const {

    return cache(position);
}

void CachedVolume::refreshCache() {

    size = volume->getSize();
    cache.resize(size);

    forI32space({0, 0, 0}, size, [this](const auto &position) {
        float value = volume->getValue(position);
        cache(position) = value;
    });
}

