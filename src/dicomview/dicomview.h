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

		QVector<DicomSceneSet *> sceneSets;
		DicomSceneSet *curentSceneSet;

	public:
		explicit DicomView(QWidget *parent = nullptr);

		~DicomView();

		DicomScene *currentDicomScene();

		void addDicomImage(const gdcm::ImageReader *reader);

		//region Getters

		const DicomToolBar &getToolBar() const;

		//endregion


	private slots:
		void activateScene(DicomScene *scene);
		void toolbarActionTrigger(DicomToolBar::Action action);
		void toolbarStateToggle(DicomToolBar::State state);
	};
}



