//
// Created by adam on 08.05.2021.
//

#include "./ValueInterpolator.h"
#include "./Volume/DicomVolume.h"

using namespace SokarAlg;

ValueInterpolator::~ValueInterpolator() {
}

const DicomVolume *ValueInterpolator::getVolume() const {
	return vv;
}

void ValueInterpolator::setVolume(const DicomVolume *newVV) {
	vv = newVV;
}

void ValueInterpolator::dicomVolumeChanged() {
}

//region help functions

void forI32space(
		const glm::i32vec3 &from,
		const glm::i32vec3 &to,
		const std::function<void(const glm::i32vec3 &pos)> &functor
) {
	glm::i32vec3 pos = from;
	for (pos.x = from.x; pos.x <= to.x; ++(pos.x)) {

		for (pos.y = from.y; pos.y <= to.y; ++(pos.y)) {

			for (pos.z = from.z; pos.z <= to.z; ++(pos.z)) {

				functor(pos);
			}
		}
	}
}

std::unique_ptr<std::vector<glm::vec3>> makeI32space(
		const glm::i32vec3 &from,
		const glm::i32vec3 &to
) {
	auto space = std::make_unique<std::vector<glm::vec3>>();

	glm::i32vec3 pos = from;

	for (pos.x = from.x; pos.x <= to.x; ++(pos.x)) {

		for (pos.y = from.y; pos.y <= to.y; ++(pos.y)) {

			for (pos.z = from.z; pos.z <= to.z; ++(pos.z)) {

				space->push_back(pos);
			}
		}
	}

	return space;
}

//endregion


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

float PolynomialValueInterpolator1::interpolate(const glm::vec3 &pos) const {
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

	forI32space(startIndex, endIndex, [&](const auto &i32pos_i) {
		float ui = vv->getTrueValue(i32pos_i);
		float Li = 1;
		glm::vec3 pos_i = i32pos_i;

		forI32space(startIndex, endIndex, [&](const auto &i32pos_k) {
			glm::vec3 pos_k = i32pos_k;

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

float PolynomialValueInterpolator2::interpolate(const glm::vec3 &pos) const {
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
				vv->getTrueValue(i32pos_i)
				*
				std::accumulate(space->begin(), space->end(), 1.f, [&](float value, const auto &i32pos_k) {
					glm::vec3 pos_k = i32pos_k;

					if (
							areSame(pos_i.x, pos_k.x) ||
							areSame(pos_i.y, pos_k.y) ||
							areSame(pos_i.z, pos_k.z)
							) {
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

// region getter & setters

const glm::i32vec3 &PolynomialValueInterpolator1::getSize() const {
	return size;
}

void PolynomialValueInterpolator1::setSize(const glm::i32vec3 &newSize) {
	size = newSize;
}

//endregion