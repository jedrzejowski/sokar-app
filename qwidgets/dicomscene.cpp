
#include "dicomscene.h"
#include "qwidgets/scenes/monochrome2.h"

using namespace Sokar;

DicomScene::DicomScene(gdcm::Image *gdcmImage) {
	setBackgroundBrush(Qt::black);

	this->gdcmImage = gdcmImage;

	initTexts();
}

DicomScene::~DicomScene() {
}

void DicomScene::resize(int w, int h) {
	maxWidth = w;
	maxHeight = h;

	setSceneRect(0, 0, w, h);

	reposItems();
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
}

void DicomScene::reposItems() {

	text11->setPos(0, 0);
	text12->setPos((maxWidth - text12->document()->size().width()) / 2, 0);
	text13->setPos(maxWidth - text13->document()->size().width(), 0);

	text21->setPos(0, (maxHeight - text21->document()->size().height()) / 2);
	text23->setPos(maxWidth - text23->document()->size().width(),
				   (maxHeight - text23->document()->size().height()) / 2);

	text31->setPos(0, maxHeight - text31->document()->size().height());
	text32->setPos((maxWidth - text32->document()->size().width()) / 2,
				   maxHeight - text32->document()->size().height());
	text33->setPos(maxWidth - text33->document()->size().width(),
				   maxHeight - text33->document()->size().height());


}

DicomScene *DicomScene::createForImg(gdcm::Image *gdcmImage) {

	switch (gdcmImage->GetPhotometricInterpretation()) {
		case gdcm::PhotometricInterpretation::MONOCHROME2:
			return new Sokar::Monochrome2DicomScene(gdcmImage);

		default:
			throw Sokar::ImageTypeNotSupportedException();
	}
}

void DicomScene::reloadPixmap() {
	auto pixmap = genQPixmap();

	if (curentPixmapItem != nullptr) {
		removeItem(curentPixmapItem);
	}

	curentPixmapItem = addPixmap(pixmap);
	curentPixmapItem->setZValue(0);
}
