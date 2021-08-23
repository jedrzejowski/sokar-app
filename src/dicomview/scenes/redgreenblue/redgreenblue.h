#pragma once

#include <QtCore>
#include <QtWidgets>

#include "SokarScene/SceneParams.hpp"
#include "SokarScene/DicomScene.hpp"

namespace Sokar::RedGreenBlue {

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