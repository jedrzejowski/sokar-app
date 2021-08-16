#pragma once

#include <chrono>

#include <QObject>

#include "sokar/pixel.h"
#include "sokar/palette.h"
#include "SokarScene/Indicator.hpp"


namespace Sokar::Monochrome {

    class Window : public QObject, public SokarScene::Indicator {
    Q_OBJECT
    protected:
        bool shouldRegen = true;
        bool inversed = false;

        Palette *palette = nullptr;

        QMenu toolbarMenu;

        std::chrono::high_resolution_clock::time_point lastChange;

    public:

        explicit Window(SokarDicom::DataConverter &dataConverter);

        typedef enum {
            IntDynamic,
            IntStatic
        } Type;

        virtual Type type() = 0;

        virtual void mvHorizontal(int v) = 0;

        virtual void mvVertical(int v) = 0;

        virtual const Pixel &getPixel(quint64 value) = 0;

        virtual bool genLUT() = 0;

        inline bool isInversed() const { return inversed; }

        QMenu *getMenu();

        void setInversed(bool inversed);

        Palette *getPalette() const { return palette; }

        void setPalette(Palette *palette);

        bool isAlive() override;

        inline auto getLastChange() const {

            return lastChange;
        }

    protected:
        virtual void genMenu() = 0;

        void updateLastChange();

    signals:
        void forceRefreshSignal();
    };
}