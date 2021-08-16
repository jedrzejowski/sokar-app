//
// Created by adam on 16.08.2021.
//

#pragma once

#include "SokarScene.hpp"
#include "Indicator.hpp"

namespace SokarScene {

    class MousePosition : public Indicator {

    public:

        MousePosition(SokarDicom::DataConverter &dataConverter);
        void reposition() override;
        bool isAlive() override;
    };

}

