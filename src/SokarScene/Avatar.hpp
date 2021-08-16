#pragma once

#include <QtCore>
#include <QtWidgets>

#include "_classdef.h"
#include "DicomScene.hpp"

namespace Ui {
    class SceneAvatar;
}

namespace SokarScene {

    class Avatar : public QWidget {
    Q_OBJECT
    private:
        DicomScene *scene;
        QPalette qPalette;
        QMenu menu;
    public:
        explicit Avatar(DicomScene *scene);

    protected:
        void mouseReleaseEvent(QMouseEvent *event) override;
        void contextMenuEvent(QContextMenuEvent *event) override;

    public:
        inline DicomScene *getScene() const {

            return scene;
        };

    public slots:
        void updateSize(int width);
    signals:
        void clicked(Avatar *avatar);
    };
}
