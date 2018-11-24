#pragma once

#include <QtCore>
#include <QtWidgets>

#include "scenes/dicomscene.h"
#include "scenes/dicomsceneset.h"

namespace Sokar {

	class FrameChooser : public QScrollArea {
	Q_OBJECT
	private:
		QWidget *content;
		QVBoxLayout *layout;
	public:
		explicit FrameChooser(QWidget *parent = nullptr);

		void addSceneSet(DicomSceneSet *sceneSet);
		void addScene(DicomScene *scene);

	signals:
		void sceneSelected(DicomScene *scene);
	};
}

