#pragma once

#include <QtCore>

#include "../../_classdef.h"
#include "../../dataset.h"
#include "../dicomscene.h"
#include "sokar/dataconventer.h"
#include "sokar/cmdseq.h"

namespace Sokar {
	class DicomSceneSet : public QObject {
	Q_OBJECT
	protected:
		QMutex qMutex;
		QVector<DicomScene *> dicomScenes;

	public:
		explicit DicomSceneSet(QObject *parent = nullptr);
		~DicomSceneSet() override;

		inline const QVector<DicomScene *> &getScenesVector() const { return dicomScenes; }

		virtual CommandSequence *getFrameSequence() = 0;

		virtual const QString &getTitle() = 0;
	};
}