
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
		sceneParams(sceneParams),
		textColor("white") {

	gdcmStringFilter.SetFile(gdcmFile);

	setBackgroundBrush(Qt::black);

	initTexts();
	initPixelSpacingIndicator();
}

DicomScene::~DicomScene() {

}

void DicomScene::initTexts() {

	text11 = addText("text11");
	text11->setDefaultTextColor(textColor);
	text11->setZValue(Z_INDICATOR);

	text12 = addText("text12");
	text12->setDefaultTextColor(textColor);
	text12->setZValue(Z_INDICATOR);

	text13 = addText("text13");
	text13->setDefaultTextColor(textColor);
	text13->setZValue(Z_INDICATOR);

	text21 = addText("text21");
	text21->setDefaultTextColor(textColor);
	text21->setZValue(Z_INDICATOR);

	text31 = addText("text31");
	text31->setDefaultTextColor(textColor);
	text31->setZValue(Z_INDICATOR);

	text33 = addText("text33");
	text33->setDefaultTextColor(textColor);
	text33->setZValue(Z_INDICATOR);
}

void DicomScene::reposItems() {

	text11->setPos(0, 0);
	text12->setPos((this->width() - text12->document()->size().width()) / 2, 0);
	text13->setPos(this->width() - text13->document()->size().width(), 0);

	text21->setPos(0,
				   (this->height() - text21->document()->size().height()) / 2);
	text31->setPos(0, this->height() - text31->document()->size().height());
	text33->setPos(this->width() - text33->document()->size().width(),
				   this->height() - text33->document()->size().height());

	if (pixmapItem != nullptr)
		pixmapItem->setPos(
				(this->width() - pixmapItem->pixmap().width()) / 2,
				(this->height() - pixmapItem->pixmap().height()) / 2);

	reposPixelSpacingIndicator();
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

void DicomScene::refreshText33() {

	text33->setHtml(genText33());

	text33->setPos(this->width() - text33->document()->size().width(),
				   this->height() - text33->document()->size().height());
}

QString DicomScene::genText33() {
	return QString();
}

void DicomScene::initPixelSpacingIndicator() {
	if (!gdcmDataSet.FindDataElement(gdcm::TagPixelSpacing)) return;

	auto pixelSpacings = QString::fromStdString(gdcmStringFilter.ToString(gdcm::TagPixelSpacing))
			.split('\\');

	bool ok = false;
	static auto pen = QPen(textColor);

	if (pixelSpacings.length() >= 1) {
		auto spacing = pixelSpacings[0].toDouble(&ok);

		if (ok) {
			auto width = gdcmImage.GetDimension(0) / 2;
			auto size = width * spacing;

			pixelSpacingLineX = addLine(0, 0, width, 0, pen);
			pixelSpacingLineX->setZValue(Z_INDICATOR);

			pixelSpacingTextX = addText("");
			pixelSpacingTextX->setDefaultTextColor(textColor);
			pixelSpacingTextX->setZValue(Z_INDICATOR);
			pixelSpacingTextX->setHtml(QString::number(size, 'f', 2) + +" <i>mm</i>");

		}
	}

	if (pixelSpacings.length() >= 2) {

		auto spacing = pixelSpacings[1].toDouble(&ok);

		if (ok) {
			auto height = gdcmImage.GetDimension(1) / 2;
			auto size = height * spacing;

			pixelSpacingLineY = addLine(0, 0, 0, height, pen);
			pixelSpacingLineY->setZValue(Z_INDICATOR);

			pixelSpacingTextY = addText("");
			pixelSpacingTextY->setDefaultTextColor(textColor);
			pixelSpacingTextY->setZValue(Z_INDICATOR);
			pixelSpacingTextY->setHtml(QString::number(size, 'f', 2) + +" <i>mm</i>");

			// Obracanie
			auto transform = QTransform();
			auto dx = pixelSpacingTextY->document()->size().width() / 2,
					dy = pixelSpacingTextY->document()->size().height() / 2;

			transform.translate(dx, dy);
			transform.rotate(90);
			transform.translate(-dx, -dx);

			pixelSpacingTextY->setTransform(transform);
		}
	}

}

void DicomScene::reposPixelSpacingIndicator() {


	if (pixelSpacingTextX != nullptr)
		pixelSpacingTextX->setPos(
				(this->width() - pixelSpacingTextX->document()->size().width()) / 2,
				this->height() - pixelSpacingTextX->document()->size().height());


	if (pixelSpacingLineX != nullptr)
		pixelSpacingLineX->setPos(
				(this->width() - pixelSpacingLineX->line().length()) / 2,
				this->height() - pixelSpacingTextX->document()->size().height());

	if (pixelSpacingTextY != nullptr)
		pixelSpacingTextY->setPos(
				this->width() - pixelSpacingTextY->document()->size().width(),
				(this->height() - pixelSpacingTextY->document()->size().height()) / 2);

	if (pixelSpacingLineY != nullptr)
		pixelSpacingLineY->setPos(
				this->width() - pixelSpacingTextY->document()->size().height(),
				(this->height() - pixelSpacingLineX->line().length()) / 2);
}
