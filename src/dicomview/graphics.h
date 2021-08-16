#pragma once

#include <QtCore>
#include <QtWidgets>

#include "_classdef.h"
#include "SokarScene.hpp"

namespace Sokar {
    class DicomGraphics : public QGraphicsView {
    Q_OBJECT
    public:
        explicit DicomGraphics(QWidget *parent);

    protected:

        void scrollContentsBy(int dx, int dy) override;

        SokarScene::DicomScene *getDicomScene() const {

            return (SokarScene::DicomScene *) scene();
        }

        void resizeEvent(QResizeEvent *event) override;
    };
}