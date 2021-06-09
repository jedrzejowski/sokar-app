#pragma once

#include <QtCore>

#include "_classdef.h"
#include "../../dataset.h"
#include "../dicomscene.h"
#include "src/SokarDicom/DataConventer.hpp"
#include "sceneseq.h"

namespace Sokar {

	typedef QVector<const gdcm::ImageReader *> DicomReaderVec;
	typedef QVector<DicomScene *> DicomSceneVec;

	class DicomSceneSet : public QObject {
	Q_OBJECT
	protected:
		QMutex qMutex;
		DicomSceneVec dicomScenes;

		SceneSequence *sceneSequence = nullptr;

	public:
		explicit DicomSceneSet(QObject *parent = nullptr);
		~DicomSceneSet() override;

		inline const DicomSceneVec &getScenesVector() const { return dicomScenes; }

		virtual SceneSequence *getSceneSequence() = 0;

		virtual const QString &getTitle() = 0;

		inline DicomScene *operator[](int index) { return dicomScenes[index]; }
	};
}