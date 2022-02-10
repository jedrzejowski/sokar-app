//
// Created by adam on 20.05.2021.
//

#include <algorithm>
#include "CachedVolume.hpp"

using namespace SokarAlg;

void CachedVolume::volumeChanged() {

    cache.resize({0, 0, 0});
}

glm::i32vec3 CachedVolume::getSize() const {

    return size;
}

float CachedVolume::getValue(const glm::i32vec3 &position) const {

    return cache[position];
}

void CachedVolume::refreshCache() {

    size = volume->getSize();
    cache.resize(size);

    if (threads_count <= 1) {
        refreshPartOfCache({0, 0, 0}, size);
        return;
    }

    enum Dim {
        X, Y, Z
    } dim;

    dim = X;
    auto dimSize = size.x;
    if (size.y > dim) {
        dim = Y;
        dimSize = size.y;
    }
    if (size.z > dim) {
        dim = Z;
        dimSize = size.z;
    }

    std::vector<std::thread> threads;

    int step = dimSize / threads_count;

    for (int i = 0, n = 0; i < threads_count; i++) {

        auto from = glm::i32vec3(0), to = size;

        switch (dim) {
            case X:
                from.x = i * step;
                to.x = (i + 1) * step;
                if (i == threads_count - 1) {
                    to.x = size.x;
                }
                break;
            case Y:
                from.y = i * step;
                to.y = (i + 1) * step;
                if (i == threads_count - 1) {
                    to.y = size.y;
                }
                break;
            case Z:
                from.z = i * step;
                to.z = (i + 1) * step;
                if (i == threads_count - 1) {
                    to.z = size.z;
                }
                break;
        }

        std::thread t(
                &CachedVolume::refreshPartOfCache, this,
                from, to
        );

        threads.push_back(std::move(t));
    }

    for (auto &t: threads) t.join();
}

void CachedVolume::refreshPartOfCache(glm::i32vec3 from, glm::i32vec3 to) {

    forI32space(from, to, [this](const auto &position) {
        float value = volume->getValue(position);
        cache[position] = value;
    });
}

int CachedVolume::getThreadsCount() const {

    return threads_count;
}

void CachedVolume::setThreadsCount(int threadsCount) {

    threads_count = threadsCount;
}

