#pragma once

#include <gdcmImageReader.h>

#include <QtCore>
#include <QtWidgets>
#include "_classdef.h"

#include "scenes/sets/_sceneset.h"
#include "toolbar.h"

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

		DicomScene *getDicomScene();
		DicomToolBar *getToolBar();
		FrameChooser *getFrameChooser();

		inline DicomSceneSet *getDicomSceneSet() { return dicomSceneSet; }

		inline const QString &getTitle() { return dicomSceneSet->getTitle(); }

		//endregion

	private slots:
		void toolbarActionTrigger(DicomToolBar::Action action);
		void toolbarStateToggle(DicomToolBar::State state);
	public slots:
		void setStep(const Step *step);
	signals:
		void stepChanged(const Step *step);
	};
}


