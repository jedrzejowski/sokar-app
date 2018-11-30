#pragma once

#include <QtCore>

#include "../_classdef.h"
#include "../dataset.h"
#include "dicomscene.h"

namespace Sokar {
	class DicomSceneSet : public QObject {
	Q_OBJECT
	private:
		QVector<DicomScene *> vector;
		std::vector<char> imageBuffer;
		const gdcm::ImageReader *imageReader;

		QVector<DataSetViewer *> dataSetViewers;

	public:
		explicit DicomSceneSet(const gdcm::ImageReader *reader, QObject *parent = nullptr);
		~DicomSceneSet() override;

		//region Getters

		inline const QVector<DicomScene *> &getVector() const {
			return vector;
		}

		inline const gdcm::File &getGdcmFile() const {
			return imageReader->GetFile();
		}

		inline QVector<DataSetViewer *> &getDataSetViewers() {
			return dataSetViewers;
		}

		//endregion

	private:

		void initScenes();
	};
}