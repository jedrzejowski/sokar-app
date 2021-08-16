#pragma once

#include <QtCore>
#include <QtWidgets>
#include <chrono>

#include <gdcmImage.h>
#include <gdcmImageReader.h>
#include <gdcmStringFilter.h>

#include "_classdef.h"

#include "dicomview/graphics.h"
#include "dicomview/toolbar.h"

#include "sokar/pixel.h"
#include "SokarGlm.hpp"
#include "sokar/exception.h"
#include "SokarScene/Scene.hpp"
#include "SokarScene/SceneParams.hpp"
#include "sceneavatar.h"
#include "moviemode.h"

#include "src/SokarScene/PatientData.hpp"
#include "src/SokarScene/PixelSpacing.hpp"
#include "src/SokarScene/HospitalData.hpp"
#include "src/SokarScene/ImageOrientation.hpp"
#include "src/SokarScene/Modality.hpp"

namespace Sokar {
    class DicomScene : public SokarScene::Scene {
    Q_OBJECT

    protected:

        QMutex processing;

        const gdcm::Image &gdcmImage;
        const gdcm::File &gdcmFile;
        const gdcm::DataSet &gdcmDataSet;
        SokarDicom::DataConverter &dataConverter;

        std::vector<char> originBuffer;
        std::vector<Pixel> targetBuffer;
        quint32 imgDimX, imgDimY;

        QImage qImage;
        QPixmap pixmap, iconPixmap;
        QGraphicsPixmapItem *pixmapItem = nullptr;
        std::chrono::high_resolution_clock::time_point lastPixmapChange;

        QTransform panTransform, scaleTransform, centerTransform, rotateTransform;

        MovieMode *movieMode = nullptr;

        //region Indicators
    private:
        void initIndicators();

        SokarScene::PatientData *patientDataIndicator = nullptr;
        SokarScene::HospitalData *hospitalDataIndicator = nullptr;
        SokarScene::PixelSpacing *pixelSpacingIndicator = nullptr;
        SokarScene::ImageOrientation *imageOrientationIndicator = nullptr;
        SokarScene::Modality *modalityIndicator = nullptr;

        void initPatientDataIndicator();
        void initPixelSpacingIndicator();
        void initImageOrientationIndicator();
        void initHospitalDataIndicator();
        void initModalityIndicator();
        //endregion

    public:

        explicit DicomScene(SokarScene::SceneParams &sceneParams);

        ~DicomScene() override;

        //region Getters

        inline const gdcm::File &getGdcmFile() { return gdcmFile; }

        inline const QPixmap *getPixmap() const { return &pixmap; }

        SceneAvatar *getAvatar();

        const QPixmap &getIcon();

        inline DicomSceneSet *getDicomSceneSet() { return (DicomSceneSet *) this->parent(); }

        DicomView *getDicomView();

        inline quint32 getImgDimX() const {

            return imgDimX;
        }

        inline quint32 getImgDimY() const {

            return imgDimY;
        }

        virtual glm::vec3 getWokselValue(quint32 x, quint32 y) const;

        //endregion

        bool saveToFile(const QString &fileName, const char *format = nullptr, int quality = -1);

        virtual void toolBarAdjust();

        bool isMovieMode();

        virtual bool acceptMovieMode(MovieMode *movieMode);

        virtual void disableMovieMode();

    protected:
        virtual bool generatePixmap() = 0;

        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

        virtual QTransform getPixmapTransformation();

        void wheelEvent(QGraphicsSceneWheelEvent *event) override;


    public slots:
        void reloadPixmap();
        virtual void toolBarActionSlot(DicomToolBar::Action action, bool state = false);
        void reposItems() override;
        void updatePixmapTransformation();
        void prepare();
        void attached();
    };
}