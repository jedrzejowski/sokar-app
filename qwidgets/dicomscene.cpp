
#include "dicomscene.h"
#include "qwidgets/scenes/header.h"

using namespace Sokar;


DicomScene::DicomScene(gdcm::Image *gdcmImage) {
//	setBackgroundBrush(Qt::black);

	this->gdcmImage = gdcmImage;

	auto qimg = ImgConvert::gdcm2Qt(*gdcmImage);
	auto pix = QPixmap::fromImage(*qimg);
	addPixmap(pix);

	initTexts();
}

DicomScene::~DicomScene() {
}

void DicomScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	QGraphicsScene::mousePressEvent(event);
}

void DicomScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	QGraphicsScene::mouseReleaseEvent(event);
}

void DicomScene::wheelEvent(QGraphicsSceneWheelEvent *event) {
	QGraphicsScene::wheelEvent(event);
}

void DicomScene::resize(int w, int h) {
	maxWidth = w;
	maxHeight = h;

	setSceneRect(0, 0, w, h);

	positTexts();
}

void DicomScene::initTexts() {
	auto txtColor = QColor("white");

	text11 = addText("text11");
	text11->setDefaultTextColor(txtColor);

	text12 = addText("text12");
	text12->setDefaultTextColor(txtColor);

	text13 = addText("text13");
	text13->setDefaultTextColor(txtColor);

	text21 = addText("text21");
	text21->setDefaultTextColor(txtColor);

	text23 = addText("text23");
	text23->setDefaultTextColor(txtColor);

	text31 = addText("text31");
	text31->setDefaultTextColor(txtColor);

	text32 = addText("text32");
	text32->setDefaultTextColor(txtColor);

	text33 = addText("text33");
	text33->setDefaultTextColor(txtColor);
}

void DicomScene::positTexts() {
	text11->setPos(0, 0);
	text12->setPos((maxWidth - text12->textWidth()) / 2, 0);
	text13->setPos(maxWidth - text13->textWidth(), 0);

	text21->setPos(0, (maxHeight - text21->document()->size().height()) / 2);
	text23->setPos(maxWidth - text13->textWidth(),
				   maxHeight - text23->document()->size().height());

	text31->setPos(0, (maxHeight - text31->document()->size().height()) / 2);
	text32->setPos((maxWidth - text12->textWidth()) / 2,
				   maxHeight - text32->document()->size().height());
	text33->setPos(maxWidth - text33->textWidth(),
				   maxHeight - text33->document()->size().height());
}

DicomScene *DicomScene::createForImg(gdcm::Image *gdcmImage) {

	switch (gdcmImage->GetPhotometricInterpretation()) {

		case gdcm::PhotometricInterpretation::MONOCHROME2:
			return new Monochrome2DicomScene(gdcmImage);

		default:
			throw Sokar::ImageTypeNotSupportedException();
	}
}

void DicomScene::refreshPixMap() {

}
