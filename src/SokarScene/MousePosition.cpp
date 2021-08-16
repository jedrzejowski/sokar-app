//
// Created by adam on 16.08.2021.
//

#include "MousePosition.hpp"

using namespace SokarScene;

MousePosition::MousePosition(SokarDicom::DataConverter &dataConverter)
        : Indicator(dataConverter) {

}

void MousePosition::reposition() {

}

bool MousePosition::isAlive() {
    return false;
}
