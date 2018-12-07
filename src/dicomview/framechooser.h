#pragma once

#include <QtCore>
#include <QtWidgets>

#include "_classdef.h"

#include "scenes/dicomscene.h"
#include "scenes/dicomsceneset.h"

namespace Ui{
	class FrameChooser;
}

namespace Sokar {

	class FrameChooser : public QWidget {
	Q_OBJECT
	private:
		Ui::FrameChooser *ui;
		QWidget *content;
		QVBoxLayout *layout;
		QVector<SceneAvatar *> avatars;
		SceneAvatar *curentAvatar;

	public:
		explicit FrameChooser(QWidget *parent = nullptr);
		virtual ~FrameChooser();

		void addSceneSet(DicomSceneSet *sceneSet);
		void addScene(DicomScene *scene);

		void moveNext();struct {
	//http://dicomiseasy.blogspot.com/2013/06/getting-oriented-using-image-plane.html?m=1
	QVector4D
			Right = QVector4D(-1, 0, 0, 1),
			Left = QVector4D(+1, 0, 0, 1),
			Head = QVector4D(0, 0, +1, 1),
			Feet = QVector4D(0, 0, -1, 1),
			Anterior = QVector4D(0, -1, 0, 1),
			Posterior = QVector4D(0, +1, 0, 1);
} PatientPoint;
		void movePrev();

	protected:
		void resizeEvent(QResizeEvent *event) override;
		void updateAvatars();

	private slots:
		void onAvatarClicked(SceneAvatar *avatar);

	signals:
		void resizeAvatars(int width);
		void selectSceneSignal(DicomScene *scene);
	};
}

