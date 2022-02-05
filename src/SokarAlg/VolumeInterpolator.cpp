//
// Created by adam on 08.05.2021.
//

#include <SokarException.hpp>
#include "./VolumeInterpolator.hpp"
#include "DicomVolume.hpp"

using namespace SokarAlg;

VolumeInterpolator::~VolumeInterpolator() {

    qDebug() << "~VolumeInterpolator()";
}

const VolumeCPtr &VolumeInterpolator::getVolume() const {

    return vv;
}

void VolumeInterpolator::setVolume(const VolumeCPtr &new_volume) {

    vv = new_volume;

    dicomVolumeChanged();
}

void VolumeInterpolator::dicomVolumeChanged() {
}

float NearestVolumeInterpolator::interpolate(const glm::vec3 &position) const {

    return vv->getValueSafe({
                                    std::round(position.x),
                                    std::round(position.y),
                                    std::round(position.z)
                            });
}

QString NearestVolumeInterpolator::toDisplay() {

    return "interpolacja do najbliższej wartości";
}

float LinearVolumeInterpolator::interpolate(const glm::vec3 &position) const {

    // https://stackoverflow.com/questions/3300290/cast-to-int-vs-floor
    glm::i32vec3 i32pos = position;

    float xd = position.x - static_cast<float>(i32pos.x);
    float yd = position.y - static_cast<float>(i32pos.y);
    float zd = position.z - static_cast<float>(i32pos.z);

    // https://en.wikipedia.org/wiki/Trilinear_interpolation

    auto c000 = vv->getValueSafe(i32pos + glm::i32vec3(0, 0, 0));
    auto c100 = vv->getValueSafe(i32pos + glm::i32vec3(1, 0, 0));
    auto c010 = vv->getValueSafe(i32pos + glm::i32vec3(0, 1, 0));
    auto c001 = vv->getValueSafe(i32pos + glm::i32vec3(0, 0, 1));
    auto c011 = vv->getValueSafe(i32pos + glm::i32vec3(0, 1, 1));
    auto c101 = vv->getValueSafe(i32pos + glm::i32vec3(1, 0, 1));
    auto c110 = vv->getValueSafe(i32pos + glm::i32vec3(1, 1, 0));
    auto c111 = vv->getValueSafe(i32pos + glm::i32vec3(1, 1, 1));

    auto c00 = c000 * (1 - xd) + c100 * xd;
    auto c01 = c001 * (1 - xd) + c101 * xd;
    auto c10 = c010 * (1 - xd) + c110 * xd;
    auto c11 = c011 * (1 - xd) + c111 * xd;

    auto c0 = c00 * (1 - yd) + c10 * yd;
    auto c1 = c01 * (1 - yd) + c11 * yd;

    return c0 * (1 - zd) + c1 * zd;
}

float PolynomialVolumeInterpolator1::interpolate(const glm::vec3 &pos) const {
    // rozwiązanie analityczne
    // https://math.stackexchange.com/a/2099510

    float u = 0;

    auto centerIndex = glm::i32vec3(
            std::round(pos.x),
            std::round(pos.y),
            std::round(pos.z)
    );

    auto startIndex = vv->clamp(centerIndex - size);
    auto endIndex = vv->clamp(centerIndex + size);

    float xAngle = M_PI / 2 - std::acos(1 / std::sqrt(size.z * size.z + 2 * size.z + 2));
    float yAngle = M_PI / 2 - std::acos(1 / std::sqrt(size.x * size.x + 2 * size.x + 2));
    float zAngle = M_PI / 2 - std::acos(1 / std::sqrt(size.y * size.y + 2 * size.y + 2));

    auto transform = glm::mat4(1);
    transform = glm::translate(transform, pos);
//    transform = glm::rotate(transform, float(M_PI / 100), glm::vec3(1, 0, 0));
//    transform = glm::rotate(transform, float(M_PI / 100), glm::vec3(0, 1, 0));
//    transform = glm::rotate(transform, float(M_PI / 100), glm::vec3(0, 0, 1));
    transform = glm::translate(transform, -pos);


    forI32space(startIndex, endIndex, [&](const auto &i32pos_i) {
        float ui = vv->getValue(i32pos_i);
        float Li = 1;
        glm::vec3 pos_i = transform * glm::vec4(i32pos_i, 1.f);

        forI32space(startIndex, endIndex, [&](const auto &i32pos_k) {
            glm::vec3 pos_k = transform * glm::vec4(i32pos_k, 1.f);

            if (
                    areSame(pos_i.x, pos_k.x) ||
                    areSame(pos_i.y, pos_k.y) ||
                    areSame(pos_i.z, pos_k.z)
                    ) {
                return;
            }

            Li = Li * (
                    (
                            (pos.x - pos_k.x) * (pos.y - pos_k.y) * (pos.z - pos_k.z)
                    ) / (
                            (pos_i.x - pos_k.x) * (pos_i.y - pos_k.y) * (pos_i.z - pos_k.z)
                    )
            );
        });

        u += ui * Li;
    });

    return u;
}

float PolynomialVolumeInterpolator2::interpolate(const glm::vec3 &pos) const {
    // rozwiązanie analityczne
    // https://math.stackexchange.com/a/2099510

    auto centerIndex = glm::i32vec3(
            std::round(pos.x),
            std::round(pos.y),
            std::round(pos.z)
    );

    auto space = makeI32space(
            vv->clamp(centerIndex - size),
            vv->clamp(centerIndex + size)
    );

    return std::accumulate(space->begin(), space->end(), 0.f, [&](float value, const auto &i32pos_i) {
        glm::vec3 pos_i = i32pos_i;

        return value + (
                vv->getValue(i32pos_i)
                *
                std::accumulate(space->begin(), space->end(), 1.f, [&](float value, const auto &i32pos_k) {
                    glm::vec3 pos_k = i32pos_k;

                    if (areSame(pos_i.x, pos_k.x) ||
                        areSame(pos_i.y, pos_k.y) ||
                        areSame(pos_i.z, pos_k.z)) {
                        return 1.f;
                    }

                    return value * (
                            (
                                    (pos.x - pos_k.x) * (pos.y - pos_k.y) * (pos.z - pos_k.z)
                            ) / (
                                    (pos_i.x - pos_k.x) * (pos_i.y - pos_k.y) * (pos_i.z - pos_k.z)
                            )
                    );
                })
        );
    });
}

QString PolynomialVolumeInterpolator2::toDisplay() {

    return "interpolacja wielomianowa (C++11)";
}

void AkimaVolumeInterpolator::dicomVolumeChanged() {

//	if (externInterpolator) {
//		delete externInterpolator;
//	}
//
//	auto trueSize = vv->getTrueSize();
//	values.resize(trueSize.x * trueSize.y * trueSize.y);
//
//	forI32space({0, 0, 0}, trueSize - 1, [&](const auto &pos) {
//		values[pos.x + trueSize.x * (pos.y + trueSize.y * pos.z)] = vv->getTrueValue(pos);
//	});
//
//	externInterpolator = new gte::IntpAkimaUniform3<float>(
//			trueSize.x, trueSize.y, trueSize.z,
//			0, vv->getWokselSize().x,
//			0, vv->getWokselSize().y,
//			0, vv->getWokselSize().z,
//			values.data()
//	);
}

float AkimaVolumeInterpolator::interpolate(const glm::vec3 &position) const {

    return externInterpolator->operator()(position.x, position.y, position.z);
}


CubicVolumeInterpolator::CubicVolumeInterpolator(bool catmullRom) {

    this->catmullRom = catmullRom;

    if (catmullRom) {
        blend[0][0] = 0.f;
        blend[0][1] = -0.5f;
        blend[0][2] = 1.f;
        blend[0][3] = -0.5f;
        blend[1][0] = 1.f;
        blend[1][1] = 0.f;
        blend[1][2] = -2.5f;
        blend[1][3] = 1.5f;
        blend[2][0] = 0.f;
        blend[2][1] = 0.5f;
        blend[2][2] = 2.f;
        blend[2][3] = -1.5f;
        blend[3][0] = 0.f;
        blend[3][1] = 0.f;
        blend[3][2] = -0.5f;
        blend[3][3] = 0.5f;
    } else {
        blend[0][0] = 1.f / 6.f;
        blend[0][1] = -3.f / 6.f;
        blend[0][2] = 3.f / 6.f;
        blend[0][3] = -1.f / 6.f;
        blend[1][0] = 4.f / 6.f;
        blend[1][1] = 0.f / 6.f;
        blend[1][2] = -6.f / 6.f;
        blend[1][3] = 3.f / 6.f;
        blend[2][0] = 1.f / 6.f;
        blend[2][1] = 3.f / 6.f;
        blend[2][2] = 3.f / 6.f;
        blend[2][3] = -3.f / 6.f;
        blend[3][0] = 0.f / 6.f;
        blend[3][1] = 0.f / 6.f;
        blend[3][2] = 0.f / 6.f;
        blend[3][3] = 1.f / 6.f;
    }
}


const int _C[64][64] = {
        { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {-3, 3, 0, 0, 0, 0, 0, 0,-2,-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 2,-2, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-2,-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {-3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-2, 0,-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0,-3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-2, 0,-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 9,-9,-9, 9, 0, 0, 0, 0, 6, 3,-6,-3, 0, 0, 0, 0, 6,-6, 3,-3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {-6, 6, 6,-6, 0, 0, 0, 0,-3,-3, 3, 3, 0, 0, 0, 0,-4, 4,-2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-2,-2,-1,-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 2, 0,-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 2, 0,-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {-6, 6, 6,-6, 0, 0, 0, 0,-4,-2, 4, 2, 0, 0, 0, 0,-3, 3,-3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-2,-1,-2,-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 4,-4,-4, 4, 0, 0, 0, 0, 2, 2,-2,-2, 0, 0, 0, 0, 2,-2, 2,-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-2,-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-3, 3, 0, 0, 0, 0, 0, 0,-2,-1, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,-2, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-2, 0,-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-2, 0,-1, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,-9,-9, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 3,-6,-3, 0, 0, 0, 0, 6,-6, 3,-3, 0, 0, 0, 0, 4, 2, 2, 1, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-6, 6, 6,-6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-3,-3, 3, 3, 0, 0, 0, 0,-4, 4,-2, 2, 0, 0, 0, 0,-2,-2,-1,-1, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0,-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0,-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-6, 6, 6,-6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-4,-2, 4, 2, 0, 0, 0, 0,-3, 3,-3, 3, 0, 0, 0, 0,-2,-1,-2,-1, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,-4,-4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2,-2,-2, 0, 0, 0, 0, 2,-2, 2,-2, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0},
        {-3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-2, 0, 0, 0,-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0,-3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-2, 0, 0, 0,-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 9,-9, 0, 0,-9, 9, 0, 0, 6, 3, 0, 0,-6,-3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6,-6, 0, 0, 3,-3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 2, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {-6, 6, 0, 0, 6,-6, 0, 0,-3,-3, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-4, 4, 0, 0,-2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-2,-2, 0, 0,-1,-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-2, 0, 0, 0,-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-3, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-2, 0, 0, 0,-1, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,-9, 0, 0,-9, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 3, 0, 0,-6,-3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6,-6, 0, 0, 3,-3, 0, 0, 4, 2, 0, 0, 2, 1, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-6, 6, 0, 0, 6,-6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-3,-3, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-4, 4, 0, 0,-2, 2, 0, 0,-2,-2, 0, 0,-1,-1, 0, 0},
        { 9, 0,-9, 0,-9, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 3, 0,-6, 0,-3, 0, 6, 0,-6, 0, 3, 0,-3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 2, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 9, 0,-9, 0,-9, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 3, 0,-6, 0,-3, 0, 6, 0,-6, 0, 3, 0,-3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 2, 0, 2, 0, 1, 0},
        {-27,27,27,-27,27,-27,-27,27,-18,-9,18, 9,18, 9,-18,-9,-18,18,-9, 9,18,-18, 9,-9,-18,18,18,-18,-9, 9, 9,-9,-12,-6,-6,-3,12, 6, 6, 3,-12,-6,12, 6,-6,-3, 6, 3,-12,12,-6, 6,-6, 6,-3, 3,-8,-4,-4,-2,-4,-2,-2,-1},
        {18,-18,-18,18,-18,18,18,-18, 9, 9,-9,-9,-9,-9, 9, 9,12,-12, 6,-6,-12,12,-6, 6,12,-12,-12,12, 6,-6,-6, 6, 6, 6, 3, 3,-6,-6,-3,-3, 6, 6,-6,-6, 3, 3,-3,-3, 8,-8, 4,-4, 4,-4, 2,-2, 4, 4, 2, 2, 2, 2, 1, 1},
        {-6, 0, 6, 0, 6, 0,-6, 0, 0, 0, 0, 0, 0, 0, 0, 0,-3, 0,-3, 0, 3, 0, 3, 0,-4, 0, 4, 0,-2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-2, 0,-2, 0,-1, 0,-1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0,-6, 0, 6, 0, 6, 0,-6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-3, 0,-3, 0, 3, 0, 3, 0,-4, 0, 4, 0,-2, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,-2, 0,-2, 0,-1, 0,-1, 0},
        {18,-18,-18,18,-18,18,18,-18,12, 6,-12,-6,-12,-6,12, 6, 9,-9, 9,-9,-9, 9,-9, 9,12,-12,-12,12, 6,-6,-6, 6, 6, 3, 6, 3,-6,-3,-6,-3, 8, 4,-8,-4, 4, 2,-4,-2, 6,-6, 6,-6, 3,-3, 3,-3, 4, 2, 4, 2, 2, 1, 2, 1},
        {-12,12,12,-12,12,-12,-12,12,-6,-6, 6, 6, 6, 6,-6,-6,-6, 6,-6, 6, 6,-6, 6,-6,-8, 8, 8,-8,-4, 4, 4,-4,-3,-3,-3,-3, 3, 3, 3, 3,-4,-4, 4, 4,-2,-2, 2, 2,-4, 4,-4, 4,-2, 2,-2, 2,-2,-2,-2,-2,-1,-1,-1,-1},
        { 2, 0, 0, 0,-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0,-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {-6, 6, 0, 0, 6,-6, 0, 0,-4,-2, 0, 0, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-3, 3, 0, 0,-3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-2,-1, 0, 0,-2,-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 4,-4, 0, 0,-4, 4, 0, 0, 2, 2, 0, 0,-2,-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,-2, 0, 0, 2,-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0,-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0,-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-6, 6, 0, 0, 6,-6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-4,-2, 0, 0, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-3, 3, 0, 0,-3, 3, 0, 0,-2,-1, 0, 0,-2,-1, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4,-4, 0, 0,-4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0,-2,-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,-2, 0, 0, 2,-2, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
        {-6, 0, 6, 0, 6, 0,-6, 0, 0, 0, 0, 0, 0, 0, 0, 0,-4, 0,-2, 0, 4, 0, 2, 0,-3, 0, 3, 0,-3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-2, 0,-1, 0,-2, 0,-1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0,-6, 0, 6, 0, 6, 0,-6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-4, 0,-2, 0, 4, 0, 2, 0,-3, 0, 3, 0,-3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0,-2, 0,-1, 0,-2, 0,-1, 0},
        {18,-18,-18,18,-18,18,18,-18,12, 6,-12,-6,-12,-6,12, 6,12,-12, 6,-6,-12,12,-6, 6, 9,-9,-9, 9, 9,-9,-9, 9, 8, 4, 4, 2,-8,-4,-4,-2, 6, 3,-6,-3, 6, 3,-6,-3, 6,-6, 3,-3, 6,-6, 3,-3, 4, 2, 2, 1, 4, 2, 2, 1},
        {-12,12,12,-12,12,-12,-12,12,-6,-6, 6, 6, 6, 6,-6,-6,-8, 8,-4, 4, 8,-8, 4,-4,-6, 6, 6,-6,-6, 6, 6,-6,-4,-4,-2,-2, 4, 4, 2, 2,-3,-3, 3, 3,-3,-3, 3, 3,-4, 4,-2, 2,-4, 4,-2, 2,-2,-2,-1,-1,-2,-2,-1,-1},
        { 4, 0,-4, 0,-4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0,-2, 0,-2, 0, 2, 0,-2, 0, 2, 0,-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        { 0, 0, 0, 0, 0, 0, 0, 0, 4, 0,-4, 0,-4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0,-2, 0,-2, 0, 2, 0,-2, 0, 2, 0,-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0},
        {-12,12,12,-12,12,-12,-12,12,-8,-4, 8, 4, 8, 4,-8,-4,-6, 6,-6, 6, 6,-6, 6,-6,-6, 6, 6,-6,-6, 6, 6,-6,-4,-2,-4,-2, 4, 2, 4, 2,-4,-2, 4, 2,-4,-2, 4, 2,-3, 3,-3, 3,-3, 3,-3, 3,-2,-1,-2,-1,-2,-1,-2,-1},
        { 8,-8,-8, 8,-8, 8, 8,-8, 4, 4,-4,-4,-4,-4, 4, 4, 4,-4, 4,-4,-4, 4,-4, 4, 4,-4,-4, 4, 4,-4,-4, 4, 2, 2, 2, 2,-2,-2,-2,-2, 2, 2,-2,-2, 2, 2,-2,-2, 2,-2, 2,-2, 2,-2, 2,-2, 1, 1, 1, 1, 1, 1, 1, 1}
};

float CubicVolumeInterpolator::interpolate(const glm::vec3 &pos) const {

    auto centerIndex = glm::i32vec3(
            std::floor(pos.x),
            std::floor(pos.y),
            std::floor(pos.z)
    );

    std::array<float, 4> U{};
    U[0] = 1.f;
    U[1] = float(centerIndex.x) - pos.x;
    U[2] = U[1] * U[1];
    U[3] = U[1] * U[2];

    std::array<float, 4> V{};
    V[0] = 1.f;
    V[1] = float(centerIndex.y) - pos.y;
    V[2] = V[1] * V[1];
    V[3] = V[1] * V[2];

    std::array<float, 4> W{};
    W[0] = 1.f;
    W[1] = float(centerIndex.z) - pos.z;
    W[2] = W[1] * W[1];
    W[3] = W[1] * W[2];

    // Compute P = M*U, Q = M*V, R = M*W.
    std::array<float, 4> P{}, Q{}, R{};
    for (int row = 0; row < 4; ++row) {
        P[row] = 0.f;
        Q[row] = 0.f;
        R[row] = 0.f;
        for (int col = 0; col < 4; ++col) {
            P[row] += blend[row][col] * U[col];
            Q[row] += blend[row][col] * V[col];
            R[row] += blend[row][col] * W[col];
        }
    }

    // Compute the tensor product (M*U)(M*V)(M*W)*D where D is the 4x4x4
    // subimage containing (x,y,z).

    centerIndex = centerIndex - 1;
    float result = 0.f;

    for (int slice = 0; slice < 4; ++slice) {

        for (int row = 0; row < 4; ++row) {

            for (int col = 0; col < 4; ++col) {

                result += P[col] * Q[row] * R[slice] *
                          vv->getValueSafe(centerIndex + glm::i32vec3({col, row, slice}));
            }
        }
    }

    // Calculate the corresponding lower-bound grid indices.
//    int xi = (int) std::floor(pos.x);
//    int yi = (int) std::floor(pos.y);
//    int zi = (int) std::floor(pos.z);
//    double _coefs[64];
//
//    // Extract the local vocal values and calculate partial derivatives.
//    double x[64] = {
//            // values of f(x,y,z) at each corner.
//            vv->getValueSafe(xi, yi, zi), vv->getValueSafe(xi + 1, yi, zi), vv->getValueSafe(xi, yi + 1, zi),
//            vv->getValueSafe(xi + 1, yi + 1, zi), vv->getValueSafe(xi, yi, zi + 1), vv->getValueSafe(xi + 1, yi, zi + 1),
//            vv->getValueSafe(xi, yi + 1, zi + 1), vv->getValueSafe(xi + 1, yi + 1, zi + 1),
//            // values of df/dx at each corner.
//            0.5 * (vv->getValueSafe(xi + 1, yi, zi) - vv->getValueSafe(xi - 1, yi, zi)),
//            0.5 * (vv->getValueSafe(xi + 2, yi, zi) - vv->getValueSafe(xi, yi, zi)),
//            0.5 * (vv->getValueSafe(xi + 1, yi + 1, zi) - vv->getValueSafe(xi - 1, yi + 1, zi)),
//            0.5 * (vv->getValueSafe(xi + 2, yi + 1, zi) - vv->getValueSafe(xi, yi + 1, zi)),
//            0.5 * (vv->getValueSafe(xi + 1, yi, zi + 1) - vv->getValueSafe(xi - 1, yi, zi + 1)),
//            0.5 * (vv->getValueSafe(xi + 2, yi, zi + 1) - vv->getValueSafe(xi, yi, zi + 1)),
//            0.5 * (vv->getValueSafe(xi + 1, yi + 1, zi + 1) - vv->getValueSafe(xi - 1, yi + 1, zi + 1)),
//            0.5 * (vv->getValueSafe(xi + 2, yi + 1, zi + 1) - vv->getValueSafe(xi, yi + 1, zi + 1)),
//            // values of df/dy at each corner.
//            0.5 * (vv->getValueSafe(xi, yi + 1, zi) - vv->getValueSafe(xi, yi - 1, zi)),
//            0.5 * (vv->getValueSafe(xi + 1, yi + 1, zi) - vv->getValueSafe(xi + 1, yi - 1, zi)),
//            0.5 * (vv->getValueSafe(xi, yi + 2, zi) - vv->getValueSafe(xi, yi, zi)),
//            0.5 * (vv->getValueSafe(xi + 1, yi + 2, zi) - vv->getValueSafe(xi + 1, yi, zi)),
//            0.5 * (vv->getValueSafe(xi, yi + 1, zi + 1) - vv->getValueSafe(xi, yi - 1, zi + 1)),
//            0.5 * (vv->getValueSafe(xi + 1, yi + 1, zi + 1) - vv->getValueSafe(xi + 1, yi - 1, zi + 1)),
//            0.5 * (vv->getValueSafe(xi, yi + 2, zi + 1) - vv->getValueSafe(xi, yi, zi + 1)),
//            0.5 * (vv->getValueSafe(xi + 1, yi + 2, zi + 1) - vv->getValueSafe(xi + 1, yi, zi + 1)),
//            // values of df/dz at each corner.
//            0.5 * (vv->getValueSafe(xi, yi, zi + 1) - vv->getValueSafe(xi, yi, zi - 1)),
//            0.5 * (vv->getValueSafe(xi + 1, yi, zi + 1) - vv->getValueSafe(xi + 1, yi, zi - 1)),
//            0.5 * (vv->getValueSafe(xi, yi + 1, zi + 1) - vv->getValueSafe(xi, yi + 1, zi - 1)),
//            0.5 * (vv->getValueSafe(xi + 1, yi + 1, zi + 1) - vv->getValueSafe(xi + 1, yi + 1, zi - 1)),
//            0.5 * (vv->getValueSafe(xi, yi, zi + 2) - vv->getValueSafe(xi, yi, zi)),
//            0.5 * (vv->getValueSafe(xi + 1, yi, zi + 2) - vv->getValueSafe(xi + 1, yi, zi)),
//            0.5 * (vv->getValueSafe(xi, yi + 1, zi + 2) - vv->getValueSafe(xi, yi + 1, zi)),
//            0.5 * (vv->getValueSafe(xi + 1, yi + 1, zi + 2) - vv->getValueSafe(xi + 1, yi + 1, zi)),
//            // values of d2f/dxdy at each corner.
//            0.25 *
//            (vv->getValueSafe(xi + 1, yi + 1, zi) - vv->getValueSafe(xi - 1, yi + 1, zi) - vv->getValueSafe(xi + 1, yi - 1, zi) +
//             vv->getValueSafe(xi - 1, yi - 1, zi)),
//            0.25 *
//            (vv->getValueSafe(xi + 2, yi + 1, zi) - vv->getValueSafe(xi, yi + 1, zi) - vv->getValueSafe(xi + 2, yi - 1, zi) +
//             vv->getValueSafe(xi, yi - 1, zi)),
//            0.25 *
//            (vv->getValueSafe(xi + 1, yi + 2, zi) - vv->getValueSafe(xi - 1, yi + 2, zi) - vv->getValueSafe(xi + 1, yi, zi) +
//             vv->getValueSafe(xi - 1, yi, zi)),
//            0.25 * (vv->getValueSafe(xi + 2, yi + 2, zi) - vv->getValueSafe(xi, yi + 2, zi) - vv->getValueSafe(xi + 2, yi, zi) +
//                    vv->getValueSafe(xi, yi, zi)),
//            0.25 * (vv->getValueSafe(xi + 1, yi + 1, zi + 1) - vv->getValueSafe(xi - 1, yi + 1, zi + 1) -
//                    vv->getValueSafe(xi + 1, yi - 1, zi + 1) + vv->getValueSafe(xi - 1, yi - 1, zi + 1)),
//            0.25 * (vv->getValueSafe(xi + 2, yi + 1, zi + 1) - vv->getValueSafe(xi, yi + 1, zi + 1) -
//                    vv->getValueSafe(xi + 2, yi - 1, zi + 1) + vv->getValueSafe(xi, yi - 1, zi + 1)),
//            0.25 * (vv->getValueSafe(xi + 1, yi + 2, zi + 1) - vv->getValueSafe(xi - 1, yi + 2, zi + 1) -
//                    vv->getValueSafe(xi + 1, yi, zi + 1) + vv->getValueSafe(xi - 1, yi, zi + 1)),
//            0.25 * (vv->getValueSafe(xi + 2, yi + 2, zi + 1) - vv->getValueSafe(xi, yi + 2, zi + 1) -
//                    vv->getValueSafe(xi + 2, yi, zi + 1) + vv->getValueSafe(xi, yi, zi + 1)),
//            // values of d2f/dxdz at each corner.
//            0.25 *
//            (vv->getValueSafe(xi + 1, yi, zi + 1) - vv->getValueSafe(xi - 1, yi, zi + 1) - vv->getValueSafe(xi + 1, yi, zi - 1) +
//             vv->getValueSafe(xi - 1, yi, zi - 1)),
//            0.25 *
//            (vv->getValueSafe(xi + 2, yi, zi + 1) - vv->getValueSafe(xi, yi, zi + 1) - vv->getValueSafe(xi + 2, yi, zi - 1) +
//             vv->getValueSafe(xi, yi, zi - 1)),
//            0.25 * (vv->getValueSafe(xi + 1, yi + 1, zi + 1) - vv->getValueSafe(xi - 1, yi + 1, zi + 1) -
//                    vv->getValueSafe(xi + 1, yi + 1, zi - 1) + vv->getValueSafe(xi - 1, yi + 1, zi - 1)),
//            0.25 * (vv->getValueSafe(xi + 2, yi + 1, zi + 1) - vv->getValueSafe(xi, yi + 1, zi + 1) -
//                    vv->getValueSafe(xi + 2, yi + 1, zi - 1) + vv->getValueSafe(xi, yi + 1, zi - 1)),
//            0.25 *
//            (vv->getValueSafe(xi + 1, yi, zi + 2) - vv->getValueSafe(xi - 1, yi, zi + 2) - vv->getValueSafe(xi + 1, yi, zi) +
//             vv->getValueSafe(xi - 1, yi, zi)),
//            0.25 * (vv->getValueSafe(xi + 2, yi, zi + 2) - vv->getValueSafe(xi, yi, zi + 2) - vv->getValueSafe(xi + 2, yi, zi) +
//                    vv->getValueSafe(xi, yi, zi)),
//            0.25 * (vv->getValueSafe(xi + 1, yi + 1, zi + 2) - vv->getValueSafe(xi - 1, yi + 1, zi + 2) -
//                    vv->getValueSafe(xi + 1, yi + 1, zi) + vv->getValueSafe(xi - 1, yi + 1, zi)),
//            0.25 * (vv->getValueSafe(xi + 2, yi + 1, zi + 2) - vv->getValueSafe(xi, yi + 1, zi + 2) -
//                    vv->getValueSafe(xi + 2, yi + 1, zi) + vv->getValueSafe(xi, yi + 1, zi)),
//            // values of d2f/dydz at each corner.
//            0.25 *
//            (vv->getValueSafe(xi, yi + 1, zi + 1) - vv->getValueSafe(xi, yi - 1, zi + 1) - vv->getValueSafe(xi, yi + 1, zi - 1) +
//             vv->getValueSafe(xi, yi - 1, zi - 1)),
//            0.25 * (vv->getValueSafe(xi + 1, yi + 1, zi + 1) - vv->getValueSafe(xi + 1, yi - 1, zi + 1) -
//                    vv->getValueSafe(xi + 1, yi + 1, zi - 1) + vv->getValueSafe(xi + 1, yi - 1, zi - 1)),
//            0.25 *
//            (vv->getValueSafe(xi, yi + 2, zi + 1) - vv->getValueSafe(xi, yi, zi + 1) - vv->getValueSafe(xi, yi + 2, zi - 1) +
//             vv->getValueSafe(xi, yi, zi - 1)),
//            0.25 * (vv->getValueSafe(xi + 1, yi + 2, zi + 1) - vv->getValueSafe(xi + 1, yi, zi + 1) -
//                    vv->getValueSafe(xi + 1, yi + 2, zi - 1) + vv->getValueSafe(xi + 1, yi, zi - 1)),
//            0.25 *
//            (vv->getValueSafe(xi, yi + 1, zi + 2) - vv->getValueSafe(xi, yi - 1, zi + 2) - vv->getValueSafe(xi, yi + 1, zi) +
//             vv->getValueSafe(xi, yi - 1, zi)),
//            0.25 * (vv->getValueSafe(xi + 1, yi + 1, zi + 2) - vv->getValueSafe(xi + 1, yi - 1, zi + 2) -
//                    vv->getValueSafe(xi + 1, yi + 1, zi) + vv->getValueSafe(xi + 1, yi - 1, zi)),
//            0.25 * (vv->getValueSafe(xi, yi + 2, zi + 2) - vv->getValueSafe(xi, yi, zi + 2) - vv->getValueSafe(xi, yi + 2, zi) +
//                    vv->getValueSafe(xi, yi, zi)),
//            0.25 * (vv->getValueSafe(xi + 1, yi + 2, zi + 2) - vv->getValueSafe(xi + 1, yi, zi + 2) -
//                    vv->getValueSafe(xi + 1, yi + 2, zi) + vv->getValueSafe(xi + 1, yi, zi)),
//            // values of d3f/dxdydz at each corner.
//            0.125 * (vv->getValueSafe(xi + 1, yi + 1, zi + 1) - vv->getValueSafe(xi - 1, yi + 1, zi + 1) -
//                     vv->getValueSafe(xi + 1, yi - 1, zi + 1) + vv->getValueSafe(xi - 1, yi - 1, zi + 1) -
//                     vv->getValueSafe(xi + 1, yi + 1, zi - 1) + vv->getValueSafe(xi - 1, yi + 1, zi - 1) +
//                     vv->getValueSafe(xi + 1, yi - 1, zi - 1) - vv->getValueSafe(xi - 1, yi - 1, zi - 1)),
//            0.125 * (vv->getValueSafe(xi + 2, yi + 1, zi + 1) - vv->getValueSafe(xi, yi + 1, zi + 1) -
//                     vv->getValueSafe(xi + 2, yi - 1, zi + 1) + vv->getValueSafe(xi, yi - 1, zi + 1) -
//                     vv->getValueSafe(xi + 2, yi + 1, zi - 1) + vv->getValueSafe(xi, yi + 1, zi - 1) +
//                     vv->getValueSafe(xi + 2, yi - 1, zi - 1) - vv->getValueSafe(xi, yi - 1, zi - 1)),
//            0.125 * (vv->getValueSafe(xi + 1, yi + 2, zi + 1) - vv->getValueSafe(xi - 1, yi + 2, zi + 1) -
//                     vv->getValueSafe(xi + 1, yi, zi + 1) + vv->getValueSafe(xi - 1, yi, zi + 1) -
//                     vv->getValueSafe(xi + 1, yi + 2, zi - 1) + vv->getValueSafe(xi - 1, yi + 2, zi - 1) +
//                     vv->getValueSafe(xi + 1, yi, zi - 1) - vv->getValueSafe(xi - 1, yi, zi - 1)),
//            0.125 * (vv->getValueSafe(xi + 2, yi + 2, zi + 1) - vv->getValueSafe(xi, yi + 2, zi + 1) -
//                     vv->getValueSafe(xi + 2, yi, zi + 1) + vv->getValueSafe(xi, yi, zi + 1) -
//                     vv->getValueSafe(xi + 2, yi + 2, zi - 1) + vv->getValueSafe(xi, yi + 2, zi - 1) +
//                     vv->getValueSafe(xi + 2, yi, zi - 1) - vv->getValueSafe(xi, yi, zi - 1)),
//            0.125 * (vv->getValueSafe(xi + 1, yi + 1, zi + 2) - vv->getValueSafe(xi - 1, yi + 1, zi + 2) -
//                     vv->getValueSafe(xi + 1, yi - 1, zi + 2) + vv->getValueSafe(xi - 1, yi - 1, zi + 2) -
//                     vv->getValueSafe(xi + 1, yi + 1, zi) + vv->getValueSafe(xi - 1, yi + 1, zi) +
//                     vv->getValueSafe(xi + 1, yi - 1, zi) - vv->getValueSafe(xi - 1, yi - 1, zi)),
//            0.125 * (vv->getValueSafe(xi + 2, yi + 1, zi + 2) - vv->getValueSafe(xi, yi + 1, zi + 2) -
//                     vv->getValueSafe(xi + 2, yi - 1, zi + 2) + vv->getValueSafe(xi, yi - 1, zi + 2) -
//                     vv->getValueSafe(xi + 2, yi + 1, zi) + vv->getValueSafe(xi, yi + 1, zi) +
//                     vv->getValueSafe(xi + 2, yi - 1, zi) - vv->getValueSafe(xi, yi - 1, zi)),
//            0.125 * (vv->getValueSafe(xi + 1, yi + 2, zi + 2) - vv->getValueSafe(xi - 1, yi + 2, zi + 2) -
//                     vv->getValueSafe(xi + 1, yi, zi + 2) + vv->getValueSafe(xi - 1, yi, zi + 2) -
//                     vv->getValueSafe(xi + 1, yi + 2, zi) + vv->getValueSafe(xi - 1, yi + 2, zi) +
//                     vv->getValueSafe(xi + 1, yi, zi) - vv->getValueSafe(xi - 1, yi, zi)),
//            0.125 * (vv->getValueSafe(xi + 2, yi + 2, zi + 2) - vv->getValueSafe(xi, yi + 2, zi + 2) -
//                     vv->getValueSafe(xi + 2, yi, zi + 2) + vv->getValueSafe(xi, yi, zi + 2) -
//                     vv->getValueSafe(xi + 2, yi + 2, zi) + vv->getValueSafe(xi, yi + 2, zi) + vv->getValueSafe(xi + 2, yi, zi) -
//                     vv->getValueSafe(xi, yi, zi))
//    };
//    // Convert voxel values and partial derivatives to interpolation coefficients.
//    for (int i = 0; i < 64; ++i) {
//        _coefs[i] = 0.0;
//        for (int j = 0; j < 64; ++j) {
//            _coefs[i] += _C[i][j] * x[j];
//        }
//    }
//    // Evaluate the interpolation within this grid voxel.
//    float dx = pos.x - xi;
//    float dy = pos.x - yi;
//    float dz = pos.x - zi;
//    int ijkn(0);
//    double dzpow(1);
//    double result(0);
//    for (int k = 0; k < 4; ++k) {
//        double dypow(1);
//        for (int j = 0; j < 4; ++j) {
//            result += dypow * dzpow *
//                      (_coefs[ijkn] + dx * (_coefs[ijkn + 1] + dx * (_coefs[ijkn + 2] + dx * _coefs[ijkn + 3])));
//            ijkn += 4;
//            dypow *= dy;
//        }
//        dzpow *= dz;
//    }

    return result;
}



// region getter & setters

const glm::i32vec3 &PolynomialVolumeInterpolator1::getSize() const {

    return size;
}

void PolynomialVolumeInterpolator1::setSize(const glm::i32vec3 &newSize) {

    size = newSize;
}

//endregion

QString LinearVolumeInterpolator::toDisplay() {

    return QString("%1").arg(
            "interpolacja liniowa"
    );
}

QString PolynomialVolumeInterpolator1::toDisplay() {

    return "interpolacja wielomianowa";
}


QString CubicVolumeInterpolator::toDisplay() {

    return QString("interpolacja sześcienna") + (catmullRom ? "z Catmull Rom" : "");
}

