#pragma once

#include <QtCore>

#include "../_classdef.h"
#include "../dataset.h"
#include "dicomscene.h"
#include "sokar/dataconventer.h"
#include "sokar/cmdseq.h"

namespace Sokar {
	class DicomSceneSet : public QObject {
	Q_OBJECT
	private:
		QVector<DicomScene *> dicomScenes;
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

		//region

		//region Getters

		inline const QVector<DicomScene *> &getScenesVector() const { return dicomScenes; }

		inline const gdcm::File &getGdcmFile() const { return imageReader->GetFile(); }

		CommandSequence* getFrameSequence();

		const QString &getTitle();

		//endregion

	private:

		void initScenes();
	};
}