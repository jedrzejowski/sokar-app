#pragma once

#include <QtCore>

#include "dicomscene.h"

namespace Sokar {
	class DicomSceneSet : public QObject, public std::vector<DicomScene *> {
	Q_OBJECT
	private:
		std::vector<char> imageBuffer;
		const gdcm::ImageReader *imageReader;
	public:
		explicit DicomSceneSet(const gdcm::ImageReader *reader);
		~DicomSceneSet() override;
	private:
		void initScenes();
	};
}