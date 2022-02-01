//
// Created by adam on 13.06.2021.
//

#pragma once

#include <QString>
#include "SokarGlm.hpp"

namespace SokarLib {

    class Displayable {
    public:
        virtual QString toDisplay() = 0;

        QString glmToString(const glm::vec3 &vec) const;
    };
}



