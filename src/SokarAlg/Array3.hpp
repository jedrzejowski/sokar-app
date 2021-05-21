// David Eberly, Geometric Tools, Redmond WA 98052
// Copyright (c) 1998-2021
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt
// https://www.geometrictools.com/License/Boost/LICENSE_1_0.txt
// Version: 4.0.2019.08.13

#pragma once

#include <cstddef>
#include <vector>

#include "./SokarAlg.hpp"

namespace SokarAlg {

//	template<typename T>
//	class Array3 {
//	private:
//		glm::i32vec3 size;
//		std::vector<T> data;
//
//	public:
//
//		Array3(const glm::i32vec3 &size, const T &t = T()) :
//				size(size), data(size.x * size.y * size.z, t) {}
//
//		Array3() : size(0) {
//		}
//
//		Array3(const Array3 &other) : size(other.size) {
//			*this = other;
//		}
//
//		Array3 &operator=(const Array3 &other) {
//			data = other.data;
//			return *this;
//		}
//
//		Array3(Array3 &&other) noexcept: size(other.size) {
//			*this = std::move(other);
//		}
//
//		Array3 &operator=(Array3 &&other) noexcept {
//			data = std::move(other.data);
//			return *this;
//		}
//
//		inline glm::i32vec3 size() const {
//			return size;
//		}
//
//		inline size_t size2() const {
//			return d2;
//		}
//
//		inline size_t size3() const {
//			return d3;
//		}

//		T &operator()(size_t i, size_t j, size_t k) {
//			return data[(i * d2 + j) * d3 + k];
//		}
//
//		const T &operator()(size_t i, size_t j, size_t k) const {
//			return data[(i * d2 + j) * d3 + k];
//		}
//
//		void resize(size_t newD1, size_t newD2, size_t newD3, const T &t = T()) {
//			d1 = newD1;
//			d2 = newD2;
//			d3 = newD3;
//
//			data.resize(d1 * d2 * d3, t);
//		}
//	};
}
