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
		DicomScene& scene;
		QPalette qPalette;
	public:
		SceneAvatar(DicomScene *scene);

	protected:
		void resizeEvent(QResizeEvent *event) override;
	};
}
