#pragma once

#include <QtCore>
#include <QtWidgets>
#include "_classdef.h"

#include "scenes/dicomsceneset.h"
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
		explicit DicomView(const gdcm::ImageReader *reader, QWidget *parent = nullptr);

		~DicomView() override;

		//region Getters

		DicomScene *currentDicomScene();
		DicomToolBar &getToolBar();
		FrameChooser &getFrameChooser();

		inline const QString &getTitle() { return dicomSceneSet->getTitle(); }

		//endregion

	private slots:
		void activateScene(DicomScene *scene);
		void toolbarActionTrigger(DicomToolBar::Action action);
		void toolbarStateToggle(DicomToolBar::State state);
	};
}



