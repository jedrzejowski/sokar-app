//
// Created by adam on 11.04.2021.
//

#include "SokarMacro.hpp"
#include "PipelineWrapper.hpp"

using namespace Sokar3D;


PipelineWrapper::PipelineWrapper() {

    sokarTrace();
}

PipelineWrapper::~PipelineWrapper() {

    sokarTrace();
}

bool PipelineWrapper::isHidden() const {

    return hidden;
}

void PipelineWrapper::setHidden(bool hidden) {

    PipelineWrapper::hidden = hidden;
}
