#pragma once

#include <QtCore>
#include <QtWidgets>

#include "SokarScene/SceneParams.hpp"

#include "SokarScene/DicomScene.hpp"
#include "../redgreenblue/redgreenblue.h"

namespace Sokar::LumBlueRed {

    class Scene : public SokarScene::DicomScene {
    Q_OBJECT
    public:
        explicit Scene(SokarScene::SceneParams &sceneParams);
    protected:
        bool generatePixmap() override;

    public:
        void toolBarAdjust() override;
    };
}