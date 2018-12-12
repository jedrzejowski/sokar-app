#include "sokar/gdcmSokar.h"
#include "sokar/qt.h"

#include "dicomsceneset.h"

#include "unsupported/unsupported.h"
#include "exception/exception.h"
#include "monochrome/monochrome.h"
#include "redgreenblue/redgreenblue.h"
#include "lumbluered/lumbluered.h"

using namespace Sokar;

DicomSceneSet::DicomSceneSet(const gdcm::ImageReader *reader, QObject *parent) :
		QObject(parent),
		imageReader(reader),
		gdcmFile(reader->GetFile()),
		gdcmDataSet(gdcmFile.GetDataSet()),
		gdcmImage(reader->GetImage()),
		dataConventer(imageReader->GetFile()) {

	initScenes();
}

DicomSceneSet::~DicomSceneSet() {
	for (auto &datasetViewer : dataSetViewers)
		delete datasetViewer;

	delete imageReader;
}

void DicomSceneSet::initScenes() {

	static gdcm::Tag TagNumberOfFrames(0x0028, 0x0008);

	if (gdcmDataSet.FindDataElement(TagNumberOfFrames))
		numberOfFrames = dataConventer.toString(TagNumberOfFrames).toInt();
	vector.resize(numberOfFrames);

	imageBuffer.resize(gdcmImage.GetBufferLength());
	gdcmImage.GetBuffer(&imageBuffer[0]);

	auto sceneParams = SceneParams();
	sceneParams.imgSize = gdcmImage.GetBufferLength() / numberOfFrames;
	sceneParams.dicomSceneSet = this;
	sceneParams.imageReader = imageReader;
	sceneParams.imageBuffer = &imageBuffer;

	for (auto &scene : vector) {

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

		} catch (Sokar::ImageTypeNotSupportedException &) {
			scene = new Sokar::Unsupported::Scene(sceneParams);
		} catch (std::exception e) {
			scene = new Sokar::ExceptionScene(sceneParams);
		}

		sceneParams.frame++;
	}
}

qreal DicomSceneSet::getFrameTime() {
	if (numberOfFrames == 1)
		return 0;

	static gdcm::Tag
			TagFrameTime(0x0018, 0x1063);

	if (gdcmDataSet.FindDataElement(TagFrameTime)) {
		auto frameTime = dataConventer.toDecimalString(TagFrameTime)[0];
		return frameTime;
	}

	return 0;
}

const QString &DicomSceneSet::getTitle() {
	if (!title.isEmpty()) return title;

	const static gdcm::Tag
			TagModality(0x0008, 0x0060),
			TagStudyDate(0x0008, 0x0020),
			TagPatientName(0x0010, 0x0010);


	if (gdcmDataSet.FindDataElement(TagPatientName)) {
		title += dataConventer.toString(TagPatientName);
	}

	if (gdcmDataSet.FindDataElement(TagModality)) {
		title += tr(" (%1)").arg(
				dataConventer.toString(TagModality)
		);
	}

	if (gdcmDataSet.FindDataElement(TagStudyDate)) {
		title += tr(" - %1").arg(
				dataConventer.toDate(TagStudyDate).toString(tr("yyyy-MM-dd"))
		);
	}


	return title;
}