#pragma once

#include <QtCore>
#include <QtWidgets>

#include "SokarUi.hpp"

namespace SokarUi {
    class DicomToolBar : public QToolBar {
    Q_OBJECT

    public:
        enum State {
            None,
            Windowing,
            Pan,
            Zoom,
            Rotate,
            Custom
        };

        enum Action {
            ClearPan,
            Fit2Screen, OriginalResolution,
            RotateRight90, RotateLeft90,
            FlipHorizontal, FlipVertical,
            ClearRotate,
            PatientData, HospitalData, ModalityData,
            OpenDataSet,
            Segmentation,
        };

    private:
        enum State state = None;

        QActionGroup *toggleActionGrp;

        struct {

            QAction *windowing,
                    *pan, *panClean,
                    *zoom, *zoomFit, *zoom11,
                    *rotate, *rotateRight, *rotateLeft,
                    *flipH, *flipV, *rotateClear,
                    *tags,
                    *segmentation;

            struct {
                QAction *all,
                        *patientData,
                        *modality,
                        *hospital;
            } indicator;
        } action;


    public:
        explicit DicomToolBar(QWidget *parent);

        //region Getters

        inline const State &getState() const { return state; }
        inline const auto getActionWindowing() const { return action.windowing; }
        inline const auto getActionPan() const { return action.pan; }
        inline const auto getActionZoom() const { return action.zoom; }
        inline const auto getActionRotate() const { return action.rotate; }
        inline const auto getActionTags() const { return action.tags; }
        inline const auto getActionSegmentation() const { return action.segmentation; }

        inline const auto getActionIndicators() const { return action.indicator.all; }
        inline const auto getActionPatientData() const { return action.indicator.patientData; }
        inline const auto getActionModality() const { return action.indicator.modality; }
        inline const auto getActionHospital() const { return action.indicator.hospital; }

        //endregion

    private:
        void initActions();

    signals:
        void stateToggleSignal(State state);
        void actionTriggerSignal(Action action, bool state = false);
    };
}


