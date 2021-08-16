#pragma once

#include <QtCore>

#include "_classdef.h"
#include "SokarScene/DicomScene.hpp"

namespace Sokar {
    class MovieMode : public QObject {
    Q_OBJECT
    protected:
        SokarScene::DicomScene *originScene;
        bool useSameWindow = false;
        bool useSameTranform = false;
    public:
        MovieMode(QObject *parent = nullptr);

        inline SokarScene::DicomScene *getOriginScene() const { return originScene; }

        void setOriginScene(SokarScene::DicomScene *orginScene);

        bool isUseSameWindow() const {

            return useSameWindow;
        }

        void setUseSameWindow(bool useSameWindow);

        bool isUseSameTranform() const {

            return useSameTranform;
        }

        void setUseSameTranform(bool useSameTranform);
    };
}

