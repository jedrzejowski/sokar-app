#pragma once

#include <QtCore>

#include "_classdef.h"
#include "dicomscene.h"

namespace Sokar {
	class MovieMode : public QObject {
	Q_OBJECT
	protected:
		DicomScene *originScene;
		bool useSameWindow = false;
		bool useSameTranform = false;
	public:
		MovieMode(QObject *parent = nullptr);

		inline DicomScene *getOriginScene() const { return originScene; }

		void setOriginScene(DicomScene *orginScene);

		bool isUseSameWindow() const {
			return useSameWindow;
		}

		void setUseSameWindow(bool useSameWindow);

		bool isUseSameTranform() const {
			return useSameTranform;
		}

		void setUseSameTranform(bool useSameTranform);
	};
}

