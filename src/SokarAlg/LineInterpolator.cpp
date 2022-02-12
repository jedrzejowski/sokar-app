//
// Created by adam on 19.09.2021.
//

#include "LineInterpolator.hpp"
#include "tk/spline.h"

using namespace SokarAlg;

void LineInterpolator::dicomVolumeChanged() {

}

const VolumeCPtr &LineInterpolator::getVolume() const {

    return volume;
}

void LineInterpolator::setVolume(const VolumeCPtr &new_volume) {

    volume = new_volume;

    dicomVolumeChanged();
}

float LineInterpolator::getIsoLevel() const {

    return iso_level;
}

void LineInterpolator::setIsoLevel(float isoLevel) {

    iso_level = isoLevel;
}

std::vector<Volume::Point> LineInterpolator::getPoints(
        const Volume::Point &p1, const Volume::Point &p2) const {

    auto delta = p2.position - p1.position;
    std::vector<Volume::Point> points;

    for (int i = -extend_point; i < extend_point + 2; i++) {
        auto pos = p1.position + i * delta;
        if (volume->isInVolume(pos)) {
            points.push_back(volume->getPoint(pos));
        }
    }

    return std::move(points);
}

int LineInterpolator::getExtendPointCount() const {

    return extend_point;
}

void LineInterpolator::setExtendPointCount(int new_point_count) {

    extend_point = new_point_count;
}

float LineInterpolator::findRoot(
        int a_, int b_,
        std::function<float(float w)> &&func
) const {

    auto a = float(a_);
    auto b = float(b_);

    switch (method) {
        case Bisection: {
            //https://www.geeksforgeeks.org/program-for-bisection-method/
            auto c = a;

            auto iter = 0;
            while (iter < max_iter && (b - a) >= SokarGlm::EPS) {
                ++iter;

                c = (a + b) / 2;

                if (func(c) == 0.0) {
                    break;

                } else if (func(c) * func(a) < 0) {
                    b = c;
                } else {
                    a = c;
                }
            }

            return c;
        }

        case FalsePosition: {
            //https://www.geeksforgeeks.org/program-for-method-of-false-position/
            auto c = a;

            for (int i = 0; i < max_iter; ++i) {
                c = (a * func(b) - b * func(a)) / (func(b) - func(a));

                if (func(c) == 0) {
                    break;
                } else if (func(c) * func(a) < 0) {
                    b = c;
                } else {
                    a = c;
                }
            }

            return c;
        }

        default:
            assert(false);
    }
}

glm::vec3 LineInterpolator::myClamp(const glm::vec3 &output, const glm::i32vec3 &a, const glm::i32vec3 &b) const {

    return {
            std::clamp(
                    output.x,
                    float(std::min(a.x, b.x)),
                    float(std::max(a.x, b.x))
            ),
            std::clamp(
                    output.y,
                    float(std::min(a.y, b.y)),
                    float(std::max(a.y, b.y))
            ),
            std::clamp(
                    output.z,
                    float(std::min(a.z, b.z)),
                    float(std::max(a.z, b.z))
            )
    };
}

LineIterationMethod LineInterpolator::getMethod() const {

    return method;
}

void LineInterpolator::setMethod(LineIterationMethod method) {

    this->method = method;
}

int LineInterpolator::getMaxIter() const {

    return max_iter;
}

void LineInterpolator::setMaxIter(int maxIter) {

    max_iter = maxIter;
}

// ----

HalfLineInterpolatorPtr HalfLineInterpolator::New() {

    return HalfLineInterpolatorPtr(new HalfLineInterpolator);
}

glm::vec3 HalfLineInterpolator::interpolate(const Volume::Point &p1, const Volume::Point &p2) const {

    return glm::vec3(p2.position + p1.position) / 2.f;
}

QString HalfLineInterpolator::toDisplay() {

    return "HalfLineInterpolator";
}

// ----

LinearLineInterpolatorPtr LinearLineInterpolator::New() {

    return LinearLineInterpolatorPtr(new LinearLineInterpolator);
}

glm::vec3 LinearLineInterpolator::interpolate(const Volume::Point &p1, const Volume::Point &p2) const {

//    if (areSame(iso_level, p1.value)) {
//        return p1.position;
//    }
//    if (areSame(iso_level, p2.value)) {
//        return p2.position;
//    }
//    if (areSame(p1.value, p2.value)) {
//        return p1.position;
//    }

    auto mu = (iso_level - p1.value) / (p2.value - p1.value);

    return glm::vec3(p1.position) + mu * glm::vec3(p2.position - p1.position);
}

QString LinearLineInterpolator::toDisplay() {

    return "interpolacja liniowa";
}

// ----

PolynomialLineInterpolatorPtr PolynomialLineInterpolator::New() {

    return PolynomialLineInterpolatorPtr(new PolynomialLineInterpolator);
}

glm::vec3 PolynomialLineInterpolator::interpolate(const Volume::Point &p1, const Volume::Point &p2) const {

    auto delta = p2.position - p1.position;
    auto points = getPoints(p1, p2);
    auto output = glm::vec3(0.f);


    // https://en.wikipedia.org/wiki/Polynomial_interpolation
    // x - wartość
    // y - pozycja

    if (delta.x == 0) {
        output.x = float(p1.position.x);
    } else {

        auto funcX = [&](float x) -> float {
            return std::accumulate(points.begin(), points.end(), 0.f, [&](float sum, const Volume::Point &point_j) {
                auto pi = std::accumulate(
                        points.begin(), points.end(), 1.f,
                        [&](float product, const Volume::Point &point_k) {
                            // hackowe sprawdzenie czy punkty są te same
                            if (&point_j == &point_k) {
                                return product;
                            }

                            return product * (x - float(point_k.position.x)) /
                                   float(point_j.position.x - point_k.position.x);
                        });

                return sum + float(point_j.value - iso_level) * pi;

            });
        };

//        auto test1 = funcX(p1.position.x);
//        auto test2 = funcX(p2.position.x);
//        auto test3 = funcX((p2.position.x + p1.position.x) / 2);

        output.x = findRoot(p1.position.x, p2.position.x, funcX);
    }

    if (delta.y == 0) {
        output.y = float(p1.position.y);
    } else {

        auto funcY = [&](float y) -> float {
            return std::accumulate(points.begin(), points.end(), 0.f, [&](float sum, const Volume::Point &point_j) {
                auto pi = std::accumulate(
                        points.begin(), points.end(), 1.f,
                        [&](float product, const Volume::Point &point_k) {
                            // hackowe sprawdzenie czy punkty są te same
                            if (&point_j == &point_k) {
                                return product;
                            }

                            return product * (y - float(point_k.position.y)) /
                                   float(point_j.position.y - point_k.position.y);
                        });
                return sum + float(point_j.value - iso_level) * pi;

            });
        };

        output.y = findRoot(p1.position.y, p2.position.y, funcY);
    }

    if (delta.z == 0) {
        output.z = float(p1.position.z);
    } else {

        auto funcZ = [&](float z) -> float {
            return std::accumulate(points.begin(), points.end(), 0.f, [&](float sum, const Volume::Point &point_j) {
                auto pi = std::accumulate(
                        points.begin(), points.end(), 1.f,
                        [&](float product, const Volume::Point &point_k) {
                            // hackowe sprawdzenie czy punkty są te same
                            if (&point_j == &point_k) {
                                return product;
                            }

                            return product * (z - float(point_k.position.z)) /
                                   float(point_j.position.z - point_k.position.z);
                        });
                return sum + float(point_j.value - iso_level) * pi;
            });
        };

        output.z = findRoot(p1.position.z, p2.position.z, funcZ);
    }


    ++passed_interpolations;

    return output;
//    return myClamp(sum, p1.position, p2.position);
}

QString PolynomialLineInterpolator::toDisplay() {

    return "interpolacja wielomianowa";
}



// ----

SplineLineInterpolatorPtr SplineLineInterpolator::New() {

    return SplineLineInterpolatorPtr(new SplineLineInterpolator);
}

glm::vec3 SplineLineInterpolator::interpolate(const Volume::Point &p1, const Volume::Point &p2) const {

    auto delta = p2.position - p1.position;
    auto points = getPoints(p1, p2);
    auto output = glm::vec3(0.f);


    if (delta.x == 0) {
        output.x = float(p1.position.x);
    } else {

        std::vector<double> X, V;

        std::sort(points.begin(), points.end(), [](const auto &p1, const auto &p2) -> bool {
            return p1.position.x < p2.position.x;
        });

        for (const auto &point: points) {
            V.emplace_back(point.value);
            X.emplace_back(point.position.x);
        }

        tk::spline s(X, V);

        output.x = findRoot(p1.position.x, p2.position.x, [&](auto x) { return s(x); });
//        output.x = (p1.position.x + p2.position.x) / 2.f;
    }

    if (delta.y == 0) {
        output.y = float(p1.position.y);
    } else {

        std::vector<double> Y, V;

        std::sort(points.begin(), points.end(), [](const auto &p1, const auto &p2) -> bool {
            return p1.position.y < p2.position.y;
        });

        for (const auto &point: points) {
            V.emplace_back(point.value);
            Y.emplace_back(point.position.y);
        }

        tk::spline s(Y, V);

        output.y = s(iso_level);

        output.y = findRoot(p1.position.y, p2.position.y, [&](auto y) { return s(y); });
//        output.y = (p1.position.y + p2.position.y) / 2.f;
    }

    if (delta.z == 0) {
        output.z = float(p1.position.z);
    } else {

        std::vector<double> Z, V;

        std::sort(points.begin(), points.end(), [](const auto &p1, const auto &p2) -> bool {
            return p1.position.z < p2.position.z;
        });

        for (const auto &point: points) {
            V.emplace_back(point.value);
            Z.emplace_back(point.position.z);
        }

        tk::spline s(Z, V);

        output.z = findRoot(p1.position.z, p2.position.z, [&](auto z) { return s(z); });
//        output.z = (p1.position.z + p2.position.z) / 2.f;
    }


    return output;
//    return myClamp(output, p1.position, p2.position);
}

QString SplineLineInterpolator::toDisplay() {

    return "interpolacja funkcjami sklejanymi";
}