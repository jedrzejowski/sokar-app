// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2021
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt
// https://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// Version: 4.0.2019.08.13

#pragma once

#include <cstddef>
#include <vector>

#include "./_def.h"

namespace SokarAlg {

	template<typename T>
	class Array3 {
	private:
		size_t d1, d2, d3;
		std::vector<T> data;

	public:

		Array3(size_t d1 = 0, size_t d2 = 0, size_t d3 = 0, const T &t = T()) :
				d1(d1), d2(d2), d3(d3), data(d1 * d2 * d3, t) {}

		Array3() : d1(0), d2(0), d3(0) {
		}

		Array3(const Array3 &other)
				: d1(other.d1), d2(other.d2), d3(other.d3) {
			*this = other;
		}

		Array3 &operator=(const Array3 &other) {
			data = other.data;
			return *this;
		}

		Array3(Array3 &&other) noexcept
				: d1(other.d1), d2(other.d2), d3(other.d3) {
			*this = std::move(other);
		}

		Array3 &operator=(Array3 &&other) noexcept {
			data = std::move(other.data);
			return *this;
		}

		inline size_t size1() const {
			return d1;
		}

		inline size_t size2() const {
			return d2;
		}

		inline size_t size3() const {
			return d3;
		}

		T &operator()(size_t i, size_t j, size_t k) {
			return data[(i * d2 + j) * d3 + k];
		}

		const T &operator()(size_t i, size_t j, size_t k) const {
			return data[(i * d2 + j) * d3 + k];
		}

		void resize(size_t newD1, size_t newD2, size_t newD3, const T &t = T()) {
			d1 = newD1;
			d2 = newD2;
			d3 = newD3;

			data.resize(d1 * d2 * d3, t);
		}
	};
}
