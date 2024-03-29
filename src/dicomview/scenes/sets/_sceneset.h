#pragma once

#include <QtCore>

#include "_classdef.h"
#include "../../dataset.h"
#include "SokarScene/DicomScene.hpp"
#include "SokarDicom/DataConventer.hpp"
#include "sceneseq.h"

namespace Sokar {

    typedef QVector<const gdcm::ImageReader *> DicomReaderVec;
    typedef QVector<SokarScene::DicomScene *> DicomSceneVec;

    class DicomSceneSet : public QObject {
    Q_OBJECT
    protected:
        QMutex qMutex;
        DicomSceneVec dicomScenes;

        Sokar::SceneSequence *sceneSequence = nullptr;

    public:
        explicit DicomSceneSet(QObject *parent = nullptr);
        ~DicomSceneSet() override;

        inline const DicomSceneVec &getScenesVector() const { return dicomScenes; }

        virtual Sokar::SceneSequence *getSceneSequence() = 0;

        virtual const QString &getTitle() = 0;

        inline SokarScene::DicomScene *operator[](int index) { return dicomScenes[index]; }
    };
}