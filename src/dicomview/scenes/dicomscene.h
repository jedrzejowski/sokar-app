#pragma once

#include <QtCore>
#include <QtWidgets>

#include <gdcmImage.h>
#include <gdcmImageReader.h>
#include <gdcmStringFilter.h>

#include "_classdef.h"

#include "dicomview/graphics.h"
#include "dicomview/toolbar.h"

#include "sokar/pixel.h"
#include "sokar/exception.h"
#include "sokar/dicombundle.h"
#include "scene.h"
#include "params.h"
#include "sceneavatar.h"
#include "moviemode.h"

#include "indicators/patientdata.h"
#include "indicators/pixelspacing.h"
#include "indicators/hospitaldata.h"
#include "indicators/imgorientation.h"
#include "indicators/modality.h"

namespace Sokar {
	class DicomScene : public Scene {
	Q_OBJECT

	protected:

		QMutex processing;

		const DicomBundle *dicomBundle;
		DataConverter &dataConverter;
		DicomSceneSet *dicomSceneSet;

		MovieMode *movieMode = nullptr;

		//region Indicators
		void initIndicators();

		PatientDataIndicator *patientDataIndicator = nullptr;
		HospitalDataIndicator *hospitalDataIndicator = nullptr;
		ModalityIndicator *modalityIndicator = nullptr;

		void initPatientDataIndicator();
		void initHospitalDataIndicator();
		void initModalityIndicator();
		//endregion

	public:

		explicit DicomScene(const DicomBundle *dicomBundle, DicomSceneSet *dicomSceneSet);

		~DicomScene() override;

		//region Getters

		inline const DicomBundle &getDicomBundle() { return *dicomBundle; }

		inline DicomSceneSet *getDicomSceneSet() { return dicomSceneSet; }

		DicomView *getDicomView();

		//endregion

		bool exportToFile(const QString &fileName, const uchar *options = nullptr) = 0;

		virtual void toolBarAdjust();

		//region MoveMode
	public:
		bool acceptMovieMode(MovieMode *movieMode);
		bool isMovieMode();
		void disableMovieMode();
	protected:
		virtual bool isMovieModeAcceptable(MovieMode *movieMode);
	signals:
		void movieModeAccepted();
		void movieModeDisabled();
		//endregion

	public slots:
		virtual void toolBarActionSlot(DicomToolBar::Action action, bool state = false);
		void reposItems() override;
		void prepare();
		void attached();
	};
}