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

		QVector<DicomSceneSet *> sceneSets;
		DicomSceneSet *curentSceneSet;

	public:
		explicit DicomView(QWidget *parent = nullptr);

		~DicomView() override;

		DicomScene *currentDicomScene();

		void addDicomFile(const gdcm::ImageReader *reader);

		//region Getters

		DicomToolBar &getToolBar();
		FrameChooser &getFrameChooser();

		const QString &getTitle() const;
		void setTitle(const QString &title);

		//endregion

	private slots:
		void activateScene(DicomScene *scene);
		void toolbarActionTrigger(DicomToolBar::Action action);
		void toolbarStateToggle(DicomToolBar::State state);

	signals:
		void titleChanged();
	};
}



