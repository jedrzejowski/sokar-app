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

    for (int i = -point_count; i < point_count + 2; i++) {
        auto pos = p1.position + i * delta;
        if (volume->isInVolume(pos)) {
            points.push_back(volume->getPoint(pos));
        }
    }

    return std::move(points);
}

int LineInterpolator::getPointCount() const {

    return point_count;
}

void LineInterpolator::setPointCount(int new_point_count) {

    point_count = new_point_count;
}

// ----

HalfLineInterpolatorPtr HalfLineInterpolator::New() {

    return HalfLineInterpolatorPtr(new HalfLineInterpolator);
}

glm::vec3 HalfLineInterpolator::interpolate(const Volume::Point &p1, const Volume::Point &p2) const {

    return glm::vec3(p2.position + p1.position) / 2.f;
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

// ----

PolynomialLineInterpolatorPtr PolynomialLineInterpolator::New() {

    return PolynomialLineInterpolatorPtr(new PolynomialLineInterpolator);
}

glm::vec3 PolynomialLineInterpolator::interpolate(const Volume::Point &p1, const Volume::Point &p2) const {

    try {
        auto delta = p2.position - p1.position;
        auto points = getPoints(p1, p2);
        auto sum = glm::vec3(0.f);

        // https://en.wikipedia.org/wiki/Polynomial_interpolation
        // x - wartość
        // y - pozycja

        if (delta.x == 0) {
            sum.x = float(p1.position.x);
        } else {
            sum.x = std::accumulate(points.begin(), points.end(), 0.f, [&](float sum, const Volume::Point &point_j) {
                return float(point_j.position.x) *
                       std::accumulate(points.begin(), points.end(), 1.f, [&](float sum, const Volume::Point &point_k) {
                           if (point_j.value == point_k.value) {
                               throw std::runtime_error("divide by zero");
                           }

                           return sum * (iso_level - point_k.value) / (point_j.value - point_k.value);
                       });
            });
        }

        if (delta.y == 0) {
            sum.y = float(p1.position.y);
        } else {
            sum.y = std::accumulate(points.begin(), points.end(), 0.f, [&](float sum, const Volume::Point &point_j) {
                return float(point_j.position.y) *
                       std::accumulate(points.begin(), points.end(), 1.f, [&](float sum, const Volume::Point &point_k) {
                           if (point_j.value == point_k.value) {
                               throw std::runtime_error("divide by zero");
                           }

                           return sum * (iso_level - point_k.value) / (point_j.value - point_k.value);
                       });
            });
        }

        if (delta.z == 0) {
            sum.z = float(p1.position.z);
        } else {
            sum.z = std::accumulate(points.begin(), points.end(), 0.f, [&](float sum, const Volume::Point &point_j) {
                return float(point_j.position.z) *
                       std::accumulate(points.begin(), points.end(), 1.f, [&](float sum, const Volume::Point &point_k) {
                           if (point_j.value == point_k.value) {
                               throw std::runtime_error("divide by zero");
                           }

                           return sum * (iso_level - point_k.value) / (point_j.value - point_k.value);
                       });
            });

        }

        ++passed_interpolations;

        return sum;

    } catch (const std::exception &e) {

        ++failed_interpolations;

        auto mu = (iso_level - p1.value) / (p2.value - p1.value);
        return glm::vec3(p1.position) + mu * glm::vec3(p2.position - p1.position);
    }

}


// ----

SplineLineInterpolatorPtr SplineLineInterpolator::New() {

    return SplineLineInterpolatorPtr(new SplineLineInterpolator);
}

glm::vec3 SplineLineInterpolator::interpolate(const Volume::Point &p1, const Volume::Point &p2) const {

    auto delta = p2.position - p1.position;
    auto points = getPoints(p1, p2);
    auto output = glm::vec3(0.f);

    std::sort(points.begin(), points.end(), [](const auto &p1, const auto &p2) -> bool {
        return p1.value > p2.value;
    });

    std::vector<double> V;
    for (const auto &point: points) {
        V.emplace_back(point.value);
    }

    if (delta.x == 0) {
        output.x = float(p1.position.x);
    } else {

        std::vector<double> X;
        for (const auto &point: points) {
            X.emplace_back(point.position.x);
        }

        tk::spline s(V, X);

        output.z = s(iso_level);
    }

    if (delta.y == 0) {
        output.y = float(p1.position.y);
    } else {

        std::vector<double> Y;
        for (const auto &point: points) {
            Y.emplace_back(point.position.y);
        }

        tk::spline s(V, Y);

        output.y = s(iso_level);
    }

    if (delta.z == 0) {
        output.z = float(p1.position.z);
    } else {

        std::vector<double> Z;
        for (const auto &point: points) {
            Z.emplace_back(point.position.y);
        }

        tk::spline s(V, Z);

        output.z = s(iso_level);
    }
//
//    std::vector<double> X = {0.1, 0.4, 1.2, 1.8, 2.0}; // must be increasing
//    std::vector<double> Y = {0.1, 0.7, 0.6, 1.1, 0.9};
//
//    tk::spline s(X, Y);
//    double x = 1.5, y = s(x), deriv = s.deriv(1, x);
//
//    printf("spline at %f is %f with derivative %f\n", x, y, deriv);

    return output;
}
