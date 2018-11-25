
#include <gdcmDict.h>
#include <gdcmDicts.h>
#include <gdcmGlobal.h>
#include <gdcmAttribute.h>

#include "sokar/gdcmSokar.h"
#include "sokar/gdcmSokar.h"

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
		ushort byteSize = gdcm::getPixelByteSize(gdcmFile);

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
	if (!generatePixmap()) return;

	if (pixmapItem == nullptr) {
		pixmapItem = addPixmap(pixmap);
		pixmapItem->setZValue(-1);
	} else {
		pixmapItem->setPixmap(pixmap);
	}

}

SceneAvatar *DicomScene::getAvatar() {
	return new SceneAvatar(this);
}


const QPixmap& DicomScene::getIcon() {
	if (iconPixmap.isNull()) {
		generatePixmap();

		iconPixmap = pixmap.scaledToWidth(128, Qt::SmoothTransformation);
	}

	return iconPixmap;
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