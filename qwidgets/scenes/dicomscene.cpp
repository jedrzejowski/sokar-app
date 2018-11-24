
#include <gdcmDict.h>
#include <gdcmDicts.h>
#include <gdcmGlobal.h>
#include <gdcmAttribute.h>

#include "sokar/dicomtags.h"

#include "dicomscene.h"

using namespace Sokar;

DicomScene::DicomScene(SceneParams &sceneParams) :
		Scene(),
		gdcmFile(sceneParams.imageReader->GetFile()),
		gdcmImage(sceneParams.imageReader->GetImage()),
		gdcmDataSet(gdcmFile.GetDataSet()) {

	gdcmStringFilter.SetFile(gdcmFile);

	dimX = gdcmImage.GetDimension(0);
	dimY = gdcmImage.GetDimension(1);
	area = dimX * dimY;

	{
		ushort byteSize = 0;
		switch (gdcmImage.GetPixelFormat()) {

			case gdcm::PixelFormat::INT8:
			case gdcm::PixelFormat::UINT8:
				byteSize = 1;
				break;

			case gdcm::PixelFormat::INT12:
			case gdcm::PixelFormat::INT16:
			case gdcm::PixelFormat::UINT12:
			case gdcm::PixelFormat::UINT16:
			case gdcm::PixelFormat::FLOAT16:
				byteSize = 2;
				break;

			case gdcm::PixelFormat::INT32:
			case gdcm::PixelFormat::UINT32:
			case gdcm::PixelFormat::FLOAT32:
				byteSize = 4;
				break;

			case gdcm::PixelFormat::INT64:
			case gdcm::PixelFormat::UINT64:
			case gdcm::PixelFormat::FLOAT64:
				byteSize = 8;
				break;

			case gdcm::PixelFormat::SINGLEBIT:
			case gdcm::PixelFormat::UNKNOWN:
			default:
				byteSize = 0;
		}

		if (byteSize != 0) {
			auto imgSize = area * byteSize;
			auto offset = sceneParams.frame * imgSize;
			originBuffer = std::vector<char>(sceneParams.imageBuffer->begin() + offset,
											 sceneParams.imageBuffer->begin() + offset + imgSize);
		}
	}

	targetBuffer.resize(area);


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