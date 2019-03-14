#include "frameset.h"

#include "../exception/exception.h"
#include "../monochrome/monochrome.h"
#include "../redgreenblue/redgreenblue.h"
#include "../lumbluered/lumbluered.h"

using namespace Sokar;

DicomFrameSet::DicomFrameSet(const DicomBundle *bundle, QObject *parent) :
		DicomSceneSet(parent),
		bundle(bundle) {

	initScenes();
}

DicomFrameSet::~DicomFrameSet() {
	delete bundle;
}

void DicomFrameSet::initScenes() {

	switch (bundle->type)

	}

	static gdcm::Tag TagNumberOfFrames(0x0028, 0x0008);

	if (gdcmDataSet.FindDataElement(TagNumberOfFrames))
		numberOfFrames = dataConverter.toString(TagNumberOfFrames).toInt();
	dicomScenes.resize(numberOfFrames);

	imageBuffer.resize(gdcmImage.GetBufferLength());
	gdcmImage.GetBuffer(&imageBuffer[0]);

	auto sceneParams = SceneParams();
	sceneParams.imgSize = gdcmImage.GetBufferLength() / numberOfFrames;
	sceneParams.dicomSceneSet = this;
	sceneParams.imageReader = imageReader;
	sceneParams.imageBuffer = &imageBuffer;
	sceneParams.dataConverter = &dataConverter;

	for (auto &scene : dicomScenes) {

		try {

			switch (gdcmImage.GetPhotometricInterpretation()) {
				case gdcm::PhotometricInterpretation::MONOCHROME1:
				case gdcm::PhotometricInterpretation::MONOCHROME2:
					scene = new Sokar::Monochrome::Scene(sceneParams);
					break;

				case gdcm::PhotometricInterpretation::RGB:
					scene = new Sokar::RedGreenBlue::Scene(sceneParams);
					break;

				case gdcm::PhotometricInterpretation::YBR_FULL:
				case gdcm::PhotometricInterpretation::YBR_FULL_422://Nope
				case gdcm::PhotometricInterpretation::YBR_PARTIAL_422://Nope
				case gdcm::PhotometricInterpretation::YBR_PARTIAL_420://Nope
				case gdcm::PhotometricInterpretation::YBR_ICT://Nope
				case gdcm::PhotometricInterpretation::YBR_RCT://Nope
					scene = new Sokar::LumBlueRed::Scene(sceneParams);
					break;

				default:
					throw Sokar::ImageTypeNotSupportedException();
			}
		} catch (Sokar::Exception &e) {
			scene = new ExceptionScene(sceneParams, e);
		}

		sceneParams.frame++;
	}
}

SceneSequence *DicomFrameSet::getSceneSequence() {
	QMutexLocker lock(&qMutex);

	if (sceneSequence != nullptr)
		return sceneSequence;

	sceneSequence = new SceneSequence(this);

	if (numberOfFrames == 1) {
		*sceneSequence << new Step(dicomScenes[0], quint64(1000 / 12));
		return sceneSequence;
	}

	static gdcm::Tag
			TagFrameIncrementPointer(0x0028, 0x0009),
	/**
	 * Nominal time (in msec) per individual frame. See Section C.7.6.5.1.1 for further explanation.
	 * Required if Frame Increment Pointer (0028,0009) points to Frame Time.
	 */        TagFrameTime(0x0018, 0x1063),
	/**
	 * An array that contains the real time increments (in msec) between frames for a Multi-frame image.
	 * See Section C.7.6.5.1.2 for further explanation.
	 * Required if Frame Increment Pointer (0028,0009) points to Frame Time Vector.
	 */        TagFrameTimeVector(0x0018, 0x1065),
	/**
	 * Number of frames per second.
	 */        TagCineRate(0x0018, 0x0040),
	/**
	 * Describes the preferred playback sequencing for a multi-frame image.
	 * Enumerated Values:
	 *   0:
	 *     Looping (1,2…n,1,2,…n,1,2,….n,…)
	 *   1:
	 *     Sweeping (1,2,…n,n-1,…2,1,2,…n,…)
	 */        TagPreferredPlaybackSequencing(0x0018, 0x1244); //TODO zaimplementować to

	if (!dataConverter.hasTagWithData(TagFrameIncrementPointer))
		return sceneSequence;

	auto frameIncPtr = dataConverter.toAttributeTag(TagFrameIncrementPointer);

	auto playback = dataConverter.hasTagWithData(TagPreferredPlaybackSequencing) ?
					dataConverter.toUShort(TagPreferredPlaybackSequencing) : 0;

	if (TagFrameTime == frameIncPtr) {
		auto frameTime = quint64(dataConverter.toDecimalString(TagFrameTime)[0]);

		for (int i = 0; i < numberOfFrames; i++)
			*sceneSequence << new Step(dicomScenes[i], frameTime);

		if (playback == 1) sceneSequence->setSweeping(true);

		return sceneSequence;
	}

	if (TagFrameTimeVector == frameIncPtr) {

		auto vec = dataConverter.toDecimalString(TagFrameTimeVector);
		quint64 i = 0;

		for (auto &time : vec)
			*sceneSequence << new Step(dicomScenes[i++], quint64(time));

		if (playback == 1) sceneSequence->setSweeping(true);

		return sceneSequence;
	}

	if (TagCineRate == frameIncPtr) {

		auto frameTime = quint64(1 / dataConverter.toDecimalString(TagCineRate)[0]);

		for (int i = 0; i < numberOfFrames; i++)
			*sceneSequence << new Step(dicomScenes[i], frameTime);

		if (playback == 1) sceneSequence->setSweeping(true);

		return sceneSequence;
	}

	qWarning("DicomSceneSet::getSceneSequence() unknown TagFrameIncrementPointer");

	return sceneSequence;
}

const QString &DicomFrameSet::getTitle() {
	if (!title.isEmpty()) return title;

	const static gdcm::Tag
			TagModality(0x0008, 0x0060),
			TagStudyDate(0x0008, 0x0020),
			TagPatientName(0x0010, 0x0010);


	if (gdcmDataSet.FindDataElement(TagPatientName)) {
		title += dataConverter.toPersonName(TagPatientName);
	}

	if (gdcmDataSet.FindDataElement(TagModality)) {
		title += tr(" (%1)").arg(
				dataConverter.toString(TagModality)
		);
	}

	if (gdcmDataSet.FindDataElement(TagStudyDate)) {
		title += tr(" - %1").arg(
				dataConverter.toDate(TagStudyDate).toString(tr("yyyy-MM-dd"))
		);
	}

	return title;
}