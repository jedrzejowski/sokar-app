//
// Created by adam on 18.09.2021.
//

#include "GradientVolume.hpp"

using namespace SokarAlg;


GradientVolume::GradientVolume() {

}

GradientVolumePtr GradientVolume::New() {

    return GradientVolumePtr(new GradientVolume);
}

glm::i32vec3 GradientVolume::getSize() const {

    return volume->getSize() - 1;
}

float GradientVolume::getValue(const glm::i32vec3 &position) const {

    std::vector<float> values;

    auto val = volume->getValue(position);

    if (directions bitand X) values.push_back(std::abs(volume->getValue(position - glm::i32vec3(1, 0, 0)) - val));
    if (directions bitand Y) values.push_back(std::abs(volume->getValue(position - glm::i32vec3(0, 1, 0)) - val));
    if (directions bitand Z) values.push_back(std::abs(volume->getValue(position - glm::i32vec3(0, 0, 1)) - val));
    if (directions bitand XY) values.push_back(std::abs(volume->getValue(position - glm::i32vec3(1, 1, 0)) - val));
    if (directions bitand XZ) values.push_back(std::abs(volume->getValue(position - glm::i32vec3(1, 0, 1)) - val));
    if (directions bitand YZ) values.push_back(std::abs(volume->getValue(position - glm::i32vec3(0, 1, 1)) - val));
    if (directions bitand XYZ) values.push_back(std::abs(volume->getValue(position - glm::i32vec3(1, 1, 1)) - val));

    switch (merge_strategy) {

        case Sum:
            return std::accumulate(values.begin(), values.end(), 0.f);

        case Highest:
            return *std::max_element(values.begin(), values.end());

        case Average:
            return std::accumulate(values.begin(), values.end(), 0.f) / float(values.size());

        case RootMeanSquare:
            return std::sqrt(
                    std::accumulate(values.begin(), values.end(), 0.f, [](float a, float b) {
                        return a + b * b;
                    }) / float(values.size())
            );

        case Median:
            std::sort(values.begin(), values.end());
            return values[values.size() / 2];

        default:
            Q_ASSERT(false);
    }
}

void GradientVolume::addDirection(GradientVolume::Direction new_direction) {

    directions = directions bitor new_direction;
}

GradientVolume::MergeStrategy GradientVolume::getMergeStrategy() const {

    return merge_strategy;
}

void GradientVolume::setMergeStrategy(GradientVolume::MergeStrategy mergeStrategy) {

    merge_strategy = mergeStrategy;
}

