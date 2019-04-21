#pragma once

#include "_sceneset.h"
#include "frameset.h"

namespace Sokar {

	class DicomFileSet : public DicomSceneSet {
	Q_OBJECT
	protected:
		DicomReaderVec imgReaders;
		QVector<DicomFrameSet *> frameSets;
		QString seriesInstanceUID;

		DicomFileSet(DicomReaderVec &vec, QObject *parent = nullptr);

	public:

		static QVector<DicomFileSet *> create(DicomReaderVec &vec, QObject *parent = nullptr);

		~DicomFileSet();

		const QString &getTitle() override;
		SceneSequence *getSceneSequence() override;
	};
}