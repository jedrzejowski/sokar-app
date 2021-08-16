#pragma once

#include <QtCore>
#include <QtWidgets>

#include "SokarScene/DicomScene.hpp"
#include "windowing/_window.h"

namespace Sokar::Monochrome {

    class Scene : public SokarScene::DicomScene {
    Q_OBJECT

    protected:
        Window *imgWindow;

        Window *getCurrentWindow();

    public:
        explicit Scene(SokarScene::SceneParams &sceneParams);

        ~Scene() override;

    protected:

        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

        bool generatePixmap() override;

        template<typename IntType>
        void genQPixmapOfType();

        template<typename IntType, typename WinClass>
        void genQPixmapOfTypeWidthWindow();

        template<typename IntType, typename WinClass>
        void genQPixmapOfTypeWidthWindowThread(quint64 from, quint64 to);

        template<typename IntType>
        void findExtremes(qreal &max, qreal &min);

        void readAttributes();

    public:
        bool acceptMovieMode(MovieMode *movieMode) override;

        void disableMovieMode() override;

        glm::vec3 getWokselValue(quint32 x, quint32 y) const override;

    public:
        void toolBarActionSlot(DicomToolBar::Action action, bool state) override;
        void toolBarAdjust() override;
    };
}