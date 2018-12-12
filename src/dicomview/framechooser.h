#pragma once

#include <QtCore>
#include <QtWidgets>

#include "_classdef.h"

#include "scenes/dicomscene.h"
#include "scenes/dicomsceneset.h"

namespace Ui {
	class FrameChooser;
}

namespace Sokar {

	class FrameChooser : public QWidget {
	Q_OBJECT
	private:
		Ui::FrameChooser *ui;
		QWidget *content;
		QVBoxLayout *layout;
		SceneAvatar *curentAvatar;
		QTimer qTimer;

		DicomSceneSet *sceneSet;
		QVector<SceneAvatar *> avatars;

	public:
		explicit FrameChooser(QWidget *parent = nullptr);
		virtual ~FrameChooser();

		void setSceneSet(DicomSceneSet *sceneSet);

	protected:
		void resizeEvent(QResizeEvent *event) override;

		void updateAvatars();
		void updateTimerUI();

		void initTimer();

	public slots:
		void moveNext();
		void movePrev();
		void onAvatarClicked(SceneAvatar *avatar);
		void timerToggle();
		void timerStop();
		void timerStart();
		void timerUpdateInterval();
		void timerStep();

	signals:
		void resizeAvatars(int width);
		void selectSceneSignal(DicomScene *scene);
	};
}

