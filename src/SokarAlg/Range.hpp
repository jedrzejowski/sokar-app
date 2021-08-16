//
// Created by adam on 18.05.2021.
//

#pragma once

namespace SokarAlg {

    template<typename Unit>
    struct Range {
        Unit from, to;

        inline bool contains(const Unit &val) const {

            return from < val && val < to;
        }

        inline bool includes(const Unit &val) const {

            return from <= val && val <= to;
        }

        inline float distance(const Unit &val) const {

            if (val < from) {
                return from - val;
            }

            if (to < val) {
                return val - to;
            }

            return -std::min(val - from, to - val);
        }

        inline float nearest(const Unit &val) const {

            if (val < from) {
                return from - val;
            }
            if (to < val) {
                return val - to;
            }

            qDebug() << "to raczej nie powinno wystąpić";
            return 0.f;
        }
    };
}
