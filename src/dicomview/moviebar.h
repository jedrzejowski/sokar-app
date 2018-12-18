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

	class MovieBar : public QWidget {
	Q_OBJECT
	protected:
		Ui::MovieBar *ui;

		QTimer frameTimer;

		DicomSceneSet *sceneSet = nullptr;
		SceneSequence *sceneSequence = nullptr;
		MovieMode *movieMode = nullptr;
	public:
		explicit MovieBar(QWidget *parent = nullptr);
		~MovieBar();

		void setSceneSet(DicomSceneSet *sceneSet);

		void updateUI();

		inline bool isRunning() { return movieMode != nullptr; };

	public slots:
		void togglePlaying();
		void stop();
		void start();
		void stepChanged(const Step *step);
	signals:
		void setStep(const Step *step);
	};
}
