#pragma once

#include <gdcmImageReader.h>

#include <QtCore>
#include <QtWidgets>
#include "_classdef.h"

#include "scenes/sets/_sceneset.h"
#include "src/SokarUi/DicomToolBar.hpp"

#include "SokarScene.hpp"

namespace Ui {
    class DicomView;
}

namespace Sokar {
    class DicomView : public QWidget {
    Q_OBJECT

    private:
        Ui::DicomView *ui = nullptr;
        QString title;

        DicomSceneSet *dicomSceneSet;

    public:
        explicit DicomView(DicomSceneSet *dicomSceneSet, QWidget *parent = nullptr);

        ~DicomView() override;

        //region Getters

        SokarScene::DicomScene *getDicomScene();
        SokarUi::DicomToolBar *getToolBar();
        FrameChooser *getFrameChooser();

        inline DicomSceneSet *getDicomSceneSet() { return dicomSceneSet; }

        inline const QString &getTitle() { return dicomSceneSet->getTitle(); }

        //endregion

    private slots:
        void toolbarActionTrigger(SokarUi::DicomToolBar::Action action, bool state = false);
        void toolbarStateToggle(SokarUi::DicomToolBar::State state);
    public slots:
        void setStep(const Step *step);
    signals:
        void stepChanged(const Step *step);
    };
}


