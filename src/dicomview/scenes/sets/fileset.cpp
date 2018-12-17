#include "fileset.h"

using namespace Sokar;

DicomFileSet::DicomFileSet(DicomReaderVec &vec, QObject *parent) : DicomSceneSet(parent) {

	static gdcm::Tag TagSeriesInstanceUID(0x0020, 0x000E);

	DataConverter converter;

	for (auto &imgReader: vec) {

		converter.setFile(imgReader->GetFile());

		if (seriesInstanceUID.isEmpty())
			seriesInstanceUID = converter.toString(TagSeriesInstanceUID);

		if (seriesInstanceUID != converter.toString(TagSeriesInstanceUID))
			throw DicomTagParseError(TagSeriesInstanceUID);

	}


	gdcm::StringFilter strFilter2;

	qSort(vec.begin(), vec.end(), [&](const gdcm::ImageReader *&a, const gdcm::ImageReader *&b) {
		static gdcm::Tag TagInstanceNumber(0x0020, 0x0013);
		int numA, numB;

		converter.setFile(a->GetFile());
		numA = converter.toIntegerString(TagInstanceNumber);

		converter.setFile(b->GetFile());
		numB = converter.toIntegerString(TagInstanceNumber);

		return numA < numB;
	});

	for (auto &imgReader: vec) {
		auto frameSet = new DicomFrameSet(imgReader, this);
		frameSets << frameSet;

		for (auto &dicomScene : frameSet->getScenesVector())
			dicomScenes << dicomScene;

	}
}

DicomFileSet::~DicomFileSet() {

}

const QString &DicomFileSet::getTitle() {
	return frameSets[0]->getTitle();
}

CommandSequence *DicomFileSet::getFrameSequence() {
	return nullptr;
}
