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
		QTimer frameTimer;

		DicomSceneSet *sceneSet;
		QVector<SceneAvatar *> avatars;
		CommandSequence* frameSequence;

	public:
		explicit FrameChooser(QWidget *parent = nullptr);
		virtual ~FrameChooser();

		void setSceneSet(DicomSceneSet *sceneSet);

	protected:
		void resizeEvent(QResizeEvent *event) override;

		void updateAvatars();

		void initTimer();

	private slots:
		void updateTimerUI();
		void onAvatarClicked(SceneAvatar *avatar);

	public slots:
		void moveNext();
		void movePrev();
		void moveTo(int i);
		void timerToggle();
		void timerStop();
		void timerStart();

	signals:
		void resizeAvatars(int width);
		void selectSceneSignal(DicomScene *scene);
	};
}

