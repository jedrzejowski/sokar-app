#pragma once

#include <QtCore>

#include "../_classdef.h"
#include "../dataset.h"
#include "dicomscene.h"
#include "sokar/dataconventer.h"

namespace Sokar {
	class DicomSceneSet : public QObject {
	Q_OBJECT
	private:
		QVector<DicomScene *> vector;
		std::vector<char> imageBuffer;

		const gdcm::ImageReader *imageReader;
		const gdcm::File &gdcmFile;
		const gdcm::DataSet &gdcmDataSet;
		const gdcm::Image &gdcmImage;
		DataConverter dataConventer;

		QString title;

		int numberOfFrames = 1;

	public:
		explicit DicomSceneSet(const gdcm::ImageReader *reader, QObject *parent = nullptr);
		~DicomSceneSet() override;

		//region Getters

		inline const QVector<DicomScene *> &getVector() const { return vector; }

		inline const gdcm::File &getGdcmFile() const { return imageReader->GetFile(); }

		qreal getFrameTime();

		const QString &getTitle();

		//endregion

	private:

		void initScenes();
	};
}