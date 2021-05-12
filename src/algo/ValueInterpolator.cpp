//
// Created by adam on 08.05.2021.
//

#include "./ValueInterpolator.h"
#include "./volume/DicomVolume.h"

using namespace SokarAlg;

ValueInterpolator::~ValueInterpolator() {
}

const DicomVolume *ValueInterpolator::getVolume() const {
	return vv;
}

void ValueInterpolator::setVolume(const DicomVolume *newVV) {
	vv = newVV;
}

float NearestValueInterpolator::interpolate(const glm::vec3 &position) const {
	return vv->getTrueValue({
								std::round(position.x),
								std::round(position.y),
								std::round(position.z)
						});
}

float LinearValueInterpolator::interpolate(const glm::vec3 &position) const {

	// https://stackoverflow.com/questions/3300290/cast-to-int-vs-floor
	glm::i32vec3 i32pos = position;

	float xd = position.x - static_cast<float>(i32pos.x);
	float yd = position.y - static_cast<float>(i32pos.y);
	float zd = position.z - static_cast<float>(i32pos.z);

	// https://en.wikipedia.org/wiki/Trilinear_interpolation

	auto c000 = vv->getTrueValueSafe(i32pos + glm::i32vec3(0, 0, 0));
	auto c100 = vv->getTrueValueSafe(i32pos + glm::i32vec3(1, 0, 0));
	auto c010 = vv->getTrueValueSafe(i32pos + glm::i32vec3(0, 1, 0));
	auto c001 = vv->getTrueValueSafe(i32pos + glm::i32vec3(0, 0, 1));
	auto c011 = vv->getTrueValueSafe(i32pos + glm::i32vec3(0, 1, 1));
	auto c101 = vv->getTrueValueSafe(i32pos + glm::i32vec3(1, 0, 1));
	auto c110 = vv->getTrueValueSafe(i32pos + glm::i32vec3(1, 1, 0));
	auto c111 = vv->getTrueValueSafe(i32pos + glm::i32vec3(1, 1, 1));

	auto c00 = c000 * (1 - xd) + c100 * xd;
	auto c01 = c001 * (1 - xd) + c101 * xd;
	auto c10 = c010 * (1 - xd) + c110 * xd;
	auto c11 = c011 * (1 - xd) + c111 * xd;

	auto c0 = c00 * (1 - yd) + c10 * yd;
	auto c1 = c01 * (1 - yd) + c11 * yd;

	return c0 * (1 - zd) + c1 * zd;
}
