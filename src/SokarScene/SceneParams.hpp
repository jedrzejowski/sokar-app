#pragma once

#include <QtCore>

#include "SokarGdcm.hpp"
#include "SokarScene.hpp"
#include "SokarDicom/DataConventer.hpp"
#include "dicomview/scenes/sets/_sceneset.h"

namespace SokarScene {
    struct SceneParams {
        ushort frame = 0;
        quint64 imgSize = 0;
        Sokar::DicomSceneSet *dicomSceneSet;
        const gdcm::ImageReader *imageReader;
        std::vector<char> *imageBuffer;
        SokarDicom::DataConverter *dataConverter;
    };
}
