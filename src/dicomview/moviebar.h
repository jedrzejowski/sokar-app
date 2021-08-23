#pragma once

#include <QtCore>
#include <QtWidgets>

#include "_classdef.h"

#include "scenes/sets/_sceneset.h"
#include "scenes/sets/sceneseq.h"


namespace Ui {
    class MovieBar;
}

namespace Sokar {

    /**
     * \class MovieBar
     * Kontrolka to zarządzania czasem w sekwencjach ramek
     */
    class MovieBar : public QWidget {
    Q_OBJECT
    protected:
        Ui::MovieBar *ui;

        QTimer frameTimer;

        DicomSceneSet *sceneSet = nullptr;
        SceneSequence *sceneSequence = nullptr;
        MovieMode *movieMode = nullptr;
    public:
        /**
         *
         * @param parent musi być to \class DicomView
         */
        explicit MovieBar(QWidget *parent = nullptr);
        ~MovieBar() override;

        /**
         * Ustawia \class Sokar::DicomSceneSet
         * @param sceneSet
         */
        void setSceneSet(DicomSceneSet *sceneSet);

        void updateUI();

        inline bool isRunning() { return movieMode != nullptr; };

        inline DicomView *getDicomView() { return (DicomView *) this->parent(); }

    public slots:
        void togglePlaying();
        void stop();
        void start();
        void stepChanged(const Step *step);
    signals:
        void setStep(const Step *step);
    };
}
