#pragma once

#include <QtCore>

#include "dicomscene.h"

namespace Sokar {
	class MovieMode : public QObject {
	Q_OBJECT
	protected:
		DicomScene *orginalScene;
	public:
		MovieMode(QObject *parent = nullptr);

		DicomScene* getOriginScene();
	};
}

