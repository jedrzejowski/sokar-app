//
// Created by adam on 09.06.2021.
//

#include "SokarMacro.hpp"
#include "RegionGrowthVolume.hpp"

using namespace SokarAlg;

static const quint8 NOT_VISITED = 0;
static const quint8 VISITED = 1;
static const quint8 PASSED = 2;

RegionGrowthVolumePtr RegionGrowthVolume::New() {

    return RegionGrowthVolumePtr(new RegionGrowthVolume);
}

const glm::i32vec3 &RegionGrowthVolume::getStartPoint() const {

    return startPoint;
}

void RegionGrowthVolume::setStartPoint(const glm::i32vec3 &newStartPoint) {

    startPoint = newStartPoint;
}

float RegionGrowthVolume::getMaskValue() const {

    return maskValue;
}

void RegionGrowthVolume::setMaskValue(float newMaskValue) {

    maskValue = newMaskValue;
}

const Range<float> &RegionGrowthVolume::getIsoLevel() const {

    return isoLevel;
}

void RegionGrowthVolume::setIsoLevel(const Range<float> &newIsoLevel) {

    isoLevel = newIsoLevel;
}

float RegionGrowthVolume::getValue(const glm::i32vec3 &position) const {

    return getMaskedValue(position);
}

float RegionGrowthVolume::getMaskedValue(const glm::i32vec3 &position) const {

    if (isVisited(position)) {
        return VolumeDecorator::getValue(position);
    }

    return maskValue;
}

float RegionGrowthVolume::getUnMaskedValue(const glm::i32vec3 &position) const {

    return VolumeDecorator::getValue(position);
}

void RegionGrowthVolume::regrowth() {

    glm::i32vec3 next, current;

    QQueue<glm::i32vec3> queue;
    mask.resize(getSize(), 0);

    static const glm::i32vec3 D1 = glm::i32vec3(1, 0, 0);
    static const glm::i32vec3 D2 = glm::i32vec3(-1, 0, 0);
    static const glm::i32vec3 D3 = glm::i32vec3(0, 1, 0);
    static const glm::i32vec3 D4 = glm::i32vec3(0, -1, 0);
    static const glm::i32vec3 D5 = glm::i32vec3(0, 0, 1);
    static const glm::i32vec3 D6 = glm::i32vec3(0, 0, -1);

    queue << startPoint;

    auto stepNext = [&](const glm::i32vec3 &direction) {
        next = current + direction;

        if (not isInVolume(next)) {
            return;
        }

        if (not isVisited(next)) {
            queue << next;
        }
    };

    while (!queue.isEmpty()) {

        current = queue.dequeue();

        if (isVisited(current)) {
            continue;
        }

        mask[current] = VISITED;

        if (isoLevel.distance(getUnMaskedValue(current)) < 0) {
            mask[current] = PASSED;
        } else {
            continue;
        }

        stepNext(D1);
        stepNext(D2);
        stepNext(D3);
        stepNext(D4);
        stepNext(D5);
        stepNext(D6);

    }
}

bool RegionGrowthVolume::isVisited(const glm::i32vec3 &position) const {

    return mask[position] not_eq NOT_VISITED;
}

QString RegionGrowthVolume::toDisplay() {

    return QString("rozrost obszaru z %1").arg(glmToString(startPoint));
}
