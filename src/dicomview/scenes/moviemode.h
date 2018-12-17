#pragma once

#include <QtCore>

#include "_classdef.h"
#include "dicomscene.h"

namespace Sokar {
	class MovieMode : public QObject {
	Q_OBJECT
	protected:
		DicomScene *originScene;
	public:
		MovieMode(QObject *parent = nullptr);

		inline DicomScene *getOriginScene() const { return originScene; }

		void setOriginScene(DicomScene *orginScene);
	};
}

