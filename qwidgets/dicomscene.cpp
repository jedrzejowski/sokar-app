
#include <gdcmDict.h>
#include <gdcmDicts.h>
#include <gdcmGlobal.h>
#include <gdcmAttribute.h>

#include "dicomscene.h"
#include "qwidgets/scenes/monochrome2.h"

using namespace Sokar;

DicomScene::DicomScene(const gdcm::ImageReader &imageReader, SceneParams *sceneParams) :
		gdcmFile(imageReader.GetFile()),
		gdcmImage(imageReader.GetImage()),
		gdcmDataSet(gdcmFile.GetDataSet()),
		sceneParams(sceneParams) {

	setBackgroundBrush(Qt::black);

	initTexts();
}

DicomScene::~DicomScene() {

}

void DicomScene::initTexts() {
	auto txtColor = QColor("white");
	int z = 1;

	text11 = addText("text11");
	text11->setDefaultTextColor(txtColor);
	text11->setZValue(++z);

	text12 = addText("text12");
	text12->setDefaultTextColor(txtColor);
	text12->setZValue(++z);

	text13 = addText("text13");
	text13->setDefaultTextColor(txtColor);
	text13->setZValue(++z);

	text21 = addText("text21");
	text21->setDefaultTextColor(txtColor);
	text21->setZValue(++z);

	text23 = addText("text23");
	text23->setDefaultTextColor(txtColor);
	text23->setZValue(++z);

	text31 = addText("text31");
	text31->setDefaultTextColor(txtColor);
	text31->setZValue(++z);

	text32 = addText("text32");
	text32->setDefaultTextColor(txtColor);
	text32->setZValue(++z);

	text33 = addText("text33");
	text33->setDefaultTextColor(txtColor);
	text33->setZValue(++z);

	refreshText33();
	QObject::connect(&(sceneParams->imgWindow), SIGNAL(centerChanged()), this, SLOT(refreshText33()));
	QObject::connect(&(sceneParams->imgWindow), SIGNAL(widthChanged()), this, SLOT(refreshText33()));
}

void DicomScene::reposItems() {

	text11->setPos(0, 0);
	text12->setPos((this->width() - text12->document()->size().width()) / 2, 0);
	text13->setPos(this->width() - text13->document()->size().width(), 0);

	text21->setPos(0,
				   (this->height() - text21->document()->size().height()) / 2);
	text23->setPos(this->width() - text23->document()->size().width(),
				   (this->height() - text23->document()->size().height()) / 2);

	text31->setPos(0, this->height() - text31->document()->size().height());
	text32->setPos((this->width() - text32->document()->size().width()) / 2,
				   this->height() - text32->document()->size().height());
	text33->setPos(this->width() - text33->document()->size().width(),
				   this->height() - text33->document()->size().height());

	if (pixmapItem != nullptr)
		pixmapItem->setPos(
				(this->width() - pixmapItem->pixmap().width()) / 2,
				(this->height() - pixmapItem->pixmap().height()) / 2);
}

void DicomScene::refreshText33() {

	QString str;
	str.append("<b>W</b> ").append(QString::number(sceneParams->imgWindow.getWidth()));
	str.append("<br>");
	str.append("<b>C</b> ").append(QString::number(sceneParams->imgWindow.getCenter()));

	text33->setHtml(str);

	text33->setPos(this->width() - text33->document()->size().width(),
				   this->height() - text33->document()->size().height());
}

DicomScene *DicomScene::createForImg(const gdcm::ImageReader &imageReader, SceneParams *sceneParams) {

	auto &image = imageReader.GetImage();

	switch (image.GetPhotometricInterpretation()) {
		case gdcm::PhotometricInterpretation::MONOCHROME2:
			return new Sokar::Monochrome2DicomScene(imageReader, sceneParams);

		default:
			throw Sokar::ImageTypeNotSupportedException();
	}
}

void DicomScene::reloadPixmap() {
	if (!genQPixmap()) return;

	if (pixmapItem == nullptr) {
		pixmapItem = addPixmap(pixmap);
		pixmapItem->setZValue(0);
	} else {
		pixmapItem->setPixmap(pixmap);
	}

}