#pragma once

#include <QtCore>

namespace Sokar {
    struct Pixel {
        quint8 red = 0, green = 0, blue = 0;

        Pixel() = default;

        Pixel(quint8 gray) : red(gray), green(gray), blue(gray) {}

        Pixel(quint8 red, quint8 green, quint8 blue) : red(red), green(green), blue(blue) {}

        explicit operator QString() const {

            return QString("Pixel(%1,%2,%3)").arg(
                    QString::number(red),
                    QString::number(green),
                    QString::number(blue));
        }
    };

}

