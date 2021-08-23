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

    template<typename T>
    class Array3 {
    private:
        glm::i32vec3 _size;
        std::vector<T> data;

        inline size_t position2index(const glm::i32vec3 &position) const {

            return (position.x * _size.y + position.y) * _size.z + position.z;
        }

    public:

        Array3(const glm::i32vec3 &size, const T &t = T()) :
                _size(size), data(size.x * size.y * size.z, t) {}

        Array3() : _size(0) {
        }

        Array3(const Array3 &other) : _size(other.size) {

            *this = other;
        }

        Array3 &operator=(const Array3 &other) {

            data = other.data;
            return *this;
        }

        Array3(Array3 &&other) noexcept: _size(other.size) {

            *this = std::move(other);
        }

        Array3 &operator=(Array3 &&other) noexcept {

            data = std::move(other.data);
            return *this;
        }

        [[nodiscard]]
        inline glm::i32vec3 size() const {

            return size;
        }

        [[nodiscard]]
        inline size_t sizeOfData() const {

            return data.size() * sizeof(T);
        }

        T &operator()(const glm::i32vec3 &position) {

            return data[position2index(position)];
        }

        const T &operator()(const glm::i32vec3 &position) const {

            return data[position2index(position)];
        }

        void resize(const glm::i32vec3 &newSize, const T &t = T()) {

            _size = newSize;
            data.resize(_size.x * _size.y * _size.z, t);
        }
    };
}
