#pragma once

#include <QtCore>
#include <QtWidgets>

#include "_classdef.h"

#include "src/SokarScene/DicomScene.hpp"
#include "scenes/sets/_sceneset.h"
#include "scenes/sets/sceneseq.h"
#include "SokarScene.hpp"

namespace Ui {
    class FrameChooser;
}

namespace Sokar {

    class FrameChooser : public QWidget {
    Q_OBJECT
    private:
        Ui::FrameChooser *ui;
        QVBoxLayout *layout;

        DicomSceneSet *sceneSet;
        SceneSequence *sceneSequence;
        SokarScene::Avatar *currentAvatar;
        QHash<SokarScene::DicomScene *, SokarScene::Avatar *> avatarsHash;
        QVector<SokarScene::Avatar *> avatarsVector;

    public:
        explicit FrameChooser(QWidget *parent = nullptr);
        virtual ~FrameChooser();

        void setSceneSet(DicomSceneSet *sceneSet);

    protected:
        void resizeEvent(QResizeEvent *event) override;

        void updateAvatars();

    private slots:
        void onAvatarClicked(SokarScene::Avatar *avatar);

    public slots:
        void moveTo(int i);
        void moveTo(SokarScene::DicomScene *scene);
        void stepChanged(const Step *step);

    signals:
        void resizeAvatars(int width);
        void setStep(const Step *step);
    };
}

