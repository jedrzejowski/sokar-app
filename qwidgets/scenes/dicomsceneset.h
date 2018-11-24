#pragma once

#include <QtCore>

#include "dicomscene.h"

namespace Sokar {
	class DicomSceneSet : public QObject {
	Q_OBJECT
	private:
		QVector<DicomScene *> vector;
		std::vector<char> imageBuffer;
		const gdcm::ImageReader *imageReader;

	public:
		explicit DicomSceneSet(const gdcm::ImageReader *reader, QObject *parent = nullptr);
		~DicomSceneSet();

		inline const QVector<DicomScene *> &getVector() const {
			return vector;
		}


	private:
		void initScenes();
	};
}