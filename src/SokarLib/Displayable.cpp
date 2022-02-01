//
// Created by adam on 13.06.2021.
//

#include "Displayable.hpp"


QString SokarLib::Displayable::glmToString(const glm::vec3 &vec) const {

    return QString("[%1; %2; %3]").arg(vec.x).arg(vec.y).arg(vec.z);
}
