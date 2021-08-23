#pragma once

#include <QtCore>

#include "pixel.h"

namespace Sokar {
    class Palette : public QObject {
    Q_OBJECT
    private:

        QString name, display;
        QVector<Pixel> pixels;

    public:
        static QVector<Palette *> &getAll();
        static Palette *fromFile(QFile &file);

        inline int size() {

            return pixels.size();
        }

        inline const Pixel &getPixel(int i) {

            i = std::max(0, std::min(size(), i));
            return pixels[i];
        }

        inline const Pixel &getPixel(qreal i) {

            return getPixel(int(i * (size() - 1)));
        }

        inline QString getDisplay() { return display; }

        inline QString getName() { return name; }

        inline const Pixel &getForeground() { return pixels.constLast(); }

        inline const Pixel &getBackground() { return pixels.constFirst(); }

        static Palette *getMono1();
        static Palette *getMono2();
    };
}