//
// Created by adam on 16.08.2021.
//

#pragma once

#include "SokarScene.hpp"
#include "Indicator.hpp"

namespace SokarScene {

    class MousePosition : public Indicator {
    private:
        QGraphicsTextItem *text;
        QPointF positionScale = QPointF(1.f, 1.f);
        QString positionUnit;
        QString zLine;
    public:

        MousePosition(SokarDicom::DataConverter &dataConverter);
        void reposition() override;
        bool isAlive() override;

        void sceneMouseMoveEvent(QGraphicsSceneMouseEvent *event, QGraphicsPixmapItem *pixmapItem);
    };

}

