#pragma once

#include <QtCore>

#include "_classdef.h"
#include "../../dataset.h"
#include "../dicomscene.h"
#include "sokar/dataconventer.h"
#include "sceneseq.h"

namespace Sokar {

	typedef QVector<const gdcm::ImageReader *> DicomReaderVec;
	typedef QVector<DicomScene *> DicomSceneVec;

	class DicomSceneSet : public QObject {
	Q_OBJECT
	protected:
		QMutex qMutex;
		DicomSceneVec dicomScenes;

	public:
		explicit DicomSceneSet(QObject *parent = nullptr);
		~DicomSceneSet() override;

		inline const QVector<DicomScene *> &getScenesVector() const { return dicomScenes; }

		virtual SceneSequence *getFrameSequence() = 0;

		virtual const QString &getTitle() = 0;

		inline DicomScene *operator[](int index) { return dicomScenes[index]; }
	};
}