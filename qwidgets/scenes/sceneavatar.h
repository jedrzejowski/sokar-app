#pragma once

#include <QtCore>
#include <QtWidgets>

#include "_classdef.h"

namespace Ui {
	class SceneAvatar;
}

namespace Sokar {

	class SceneAvatar : public QWidget {
	Q_OBJECT
	private:
		DicomScene *scene;
		QPalette qPalette;
	public:
		explicit SceneAvatar(DicomScene *scene);

	protected:
		void mouseReleaseEvent(QMouseEvent *event) override;
		void contextMenuEvent(QContextMenuEvent *event) override;

	public:
		inline DicomScene *getScene() const {
			return scene;
		};

	public slots:
		void updateSize(int width);
	signals:
		void selectScene(SceneAvatar *avatar);
	};
}
