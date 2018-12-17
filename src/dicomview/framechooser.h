#pragma once

#include <QtCore>
#include <QtWidgets>

#include "_classdef.h"

#include "scenes/dicomscene.h"
#include "scenes/sets/_sceneset.h"
#include "scenes/sets/sceneseq.h"

namespace Ui {
	class FrameChooser;
}

namespace Sokar {

	class FrameChooser : public QWidget {
	Q_OBJECT
	private:
		Ui::FrameChooser *ui;
		QVBoxLayout *layout;
		QTimer frameTimer;

		SceneAvatar *currentAvatar;

		QHash<DicomScene *, SceneAvatar *> avatarsHash;
		QVector<SceneAvatar *> avatarsVector;

		DicomSceneSet *sceneSet = nullptr;
		SceneSequence *frameSequence = nullptr;
		MovieMode *movieMode = nullptr;

	public:
		explicit FrameChooser(QWidget *parent = nullptr);
		virtual ~FrameChooser();

		void setSceneSet(DicomSceneSet *sceneSet);

		inline bool isMovieMode() { return movieMode != nullptr; }

	protected:
		void resizeEvent(QResizeEvent *event) override;

		void updateAvatars();

		void setupFrameMovieMode();

	private slots:
		void updateMovieModeUI();
		void onAvatarClicked(SceneAvatar *avatar);

	public slots:
		void moveNext();
		void movePrev();
		void moveTo(int i);
		void moveTo(DicomScene *scene);
		void toggleMovieMode();
		void stopMovieMode();
		void startMovieMode();

	signals:
		void resizeAvatars(int width);
		void selectSceneSignal(DicomScene *scene);
	};
}

