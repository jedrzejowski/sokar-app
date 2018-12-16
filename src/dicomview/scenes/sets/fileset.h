#pragma once

#include "_sceneset.h"
#include "frameset.h"

namespace Sokar {

	typedef QVector<const gdcm::ImageReader *> DicomReaderVec;

	class DicomFileSet : public DicomSceneSet {
	Q_OBJECT
	protected:
		QVector<DicomFrameSet *> frameSets;

	public:
		DicomFileSet(DicomReaderVec &vec, QObject *parent = nullptr);
		~DicomFileSet();
	};
}