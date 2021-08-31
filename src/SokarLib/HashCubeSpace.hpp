//
// Created by adam on 26.08.2021.
//

#pragma once

#include "SokarMacro.hpp"
#include "SokarGlm.hpp"

namespace SokarLib {

    template<typename T>
    class HashCubeSpace {
        using Index = glm::int32;
        using Index3 = glm::i32vec3;
    private:
        mutable QHash<Index, QHash<Index, QHash<Index, T>>> cluster;

    public:

        inline const T &at(const Index &index1, const Index &index2, const Index &index3) const {

            return cluster[index1][index2][index3];
        }

        inline const T &operator[](const Index3 &index) const {

            return cluster[index.x][index.y][index.z];
        }

        inline T &at(const Index &index1, const Index &index2, const Index &index3) {

            return cluster[index1][index2][index3];
        }

        inline T &operator[](const Index3 &index) {

            return cluster[index.x][index.y][index.z];
        }

        inline void forEach(std::function<void(const Index3 &index, T &value)> functor) {

            Index3 index;

            auto &xspace = cluster;
            for (auto itX = xspace.begin(); itX != xspace.end(); ++itX) {
                auto &yspace = itX.value();
                index.x = itX.key();

                for (auto itY = yspace.begin(); itY != yspace.end(); ++itY) {
                    auto &zspace = itY.value();
                    index.y = itY.key();

                    for (auto itZ = zspace.begin(); itZ != zspace.end(); ++itZ) {
                        index.z = itZ.key();

                        functor(index, itZ.value());
                    }
                }
            }
        }

        inline void constForEach(std::function<void(const Index3 &index, const T &value)> functor) const {

            Index3 index;

            auto &xspace = cluster;
            for (auto itX = xspace.begin(); itX != xspace.end(); ++itX) {
                auto &yspace = itX.value();
                index.x = itX.key();

                for (auto itY = yspace.begin(); itY != yspace.end(); ++itY) {
                    auto &zspace = itY.value();
                    index.y = itY.key();

                    for (auto itZ = zspace.begin(); itZ != zspace.end(); ++itZ) {
                        index.z = itZ.key();

                        functor(index, itZ.value());
                    }
                }
            }
        }
    };
}



