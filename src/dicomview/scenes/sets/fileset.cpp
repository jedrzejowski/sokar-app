#include "fileset.h"

using namespace Sokar;

QVector<DicomFileSet *> DicomFileSet::create(DicomReaderVec &vec, QObject* parent) {
	static gdcm::Tag TagSeriesInstanceUID(0x0020, 0x000E);

	QHash<QString, DicomReaderVec> hash;
	SokarDicom::DataConverter converter;

	for (auto &imgReader: vec) {

		converter.setFile(imgReader->GetFile());

		auto seriesInstanceUID = converter.toString(TagSeriesInstanceUID);

		if (!hash.contains(seriesInstanceUID))
			hash[seriesInstanceUID] = DicomReaderVec();

		hash[seriesInstanceUID] << imgReader;
	}

	QVector<DicomFileSet *> fileSets;

	for (auto readers : hash)
		fileSets << new DicomFileSet(readers, parent);

	return fileSets;
}


DicomFileSet::DicomFileSet(DicomReaderVec &vec, QObject *parent) : DicomSceneSet(parent) {

	SokarDicom::DataConverter converter;

	std::sort(vec.begin(), vec.end(), [&](const gdcm::ImageReader *&a, const gdcm::ImageReader *&b) {
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

SceneSequence *DicomFileSet::getSceneSequence() {
	if (sceneSequence != nullptr)
		return sceneSequence;

	sceneSequence = new SceneSequence(this);

	for (auto &frameSet : frameSets)
		*sceneSequence << frameSet->getSceneSequence();

	sceneSequence->setSweeping(true);

	return sceneSequence;
}
