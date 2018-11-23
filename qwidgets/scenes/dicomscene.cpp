
#include <gdcmDict.h>
#include <gdcmDicts.h>
#include <gdcmGlobal.h>
#include <gdcmAttribute.h>

#include "sokar/dicomtags.h"

#include "dicomscene.h"

#include "monochrome2/monochrome2.h"
#include "unsupported/unsupported.h"

using namespace Sokar;

DicomScene::DicomScene(const gdcm::ImageReader &imageReader, SceneParams *sceneParams) :
		Scene(),
		gdcmFile(imageReader.GetFile()),
		gdcmImage(imageReader.GetImage()),
		gdcmDataSet(gdcmFile.GetDataSet()),
		sceneParams(sceneParams) {

	gdcmStringFilter.SetFile(gdcmFile);

	initIndicators();
}

DicomScene::~DicomScene() {

}

void DicomScene::reposItems() {
	Scene::reposItems();

	if (pixmapItem != nullptr)
		pixmapItem->setPos(
				(this->width() - pixmapItem->pixmap().width()) / 2,
				(this->height() - pixmapItem->pixmap().height()) / 2);

}


DicomScene *DicomScene::createForImg(const gdcm::ImageReader &imageReader, SceneParams *sceneParams) {


	try {
		auto &image = imageReader.GetImage();

		switch (image.GetPhotometricInterpretation()) {
			case gdcm::PhotometricInterpretation::MONOCHROME2:
				return new Sokar::Monochrome2::Scene(imageReader, sceneParams);

			default:
				throw Sokar::ImageTypeNotSupportedException();
		}

	} catch (Sokar::ImageTypeNotSupportedException &) {
		return new Sokar::Unsupported::Scene(imageReader, sceneParams);
	}
}

void DicomScene::reloadPixmap() {
	if (!genQPixmap()) return;

	if (pixmapItem == nullptr) {
		pixmapItem = addPixmap(pixmap);
		pixmapItem->setZValue(Z_IMAGE);
	} else {
		pixmapItem->setPixmap(pixmap);
	}

}


//region Indicators

void DicomScene::initIndicators() {
	initPixelSpacingIndicator();
	initImageOrientationIndicator();
}

void DicomScene::initPixelSpacingIndicator() {
	if (!gdcmDataSet.FindDataElement(gdcm::TagPixelSpacing)) return;

	auto pixelSpacings = QString::fromStdString(gdcmStringFilter.ToString(gdcm::TagPixelSpacing))
			.split(gdcm::StringSplitter);

	bool ok = false;

	pixelSpacingIndicator = new PixelSpacingIndicator();
	addIndicator(pixelSpacingIndicator);

	if (pixelSpacings.length() >= 1) {
		auto spacing = pixelSpacings[0].toDouble(&ok);

		if (ok) {
			pixelSpacingIndicator->setXSpacing(spacing);
			pixelSpacingIndicator->setXDim(gdcmImage.GetDimension(0));
		}
	}

	if (pixelSpacings.length() >= 2) {

		auto spacing = pixelSpacings[1].toDouble(&ok);

		if (ok) {
			pixelSpacingIndicator->setYSpacing(spacing);
			pixelSpacingIndicator->setYDim(gdcmImage.GetDimension(0));
		}
	}
}

void DicomScene::initImageOrientationIndicator() {
	if (!gdcmDataSet.FindDataElement(gdcm::TagImageOrientationPatient)) return;

	auto imgTypes = QString::fromStdString(gdcmStringFilter.ToString(gdcm::TagImageType))
			.split(gdcm::StringSplitter);

	if (!imgTypes.contains("AXIAL", Qt::CaseSensitivity::CaseInsensitive)) return;

	auto orientations = QString::fromStdString(gdcmStringFilter.ToString(gdcm::TagPixelSpacing));

	imageOrientationIndicator = new ImageOrientationIndicator();
	imageOrientationIndicator->setOrientation(orientations);
	addIndicator(imageOrientationIndicator);
}

//endregion