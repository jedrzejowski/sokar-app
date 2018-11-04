#include <convert/convert.h>
#include "dicomscene.h"

using namespace Sokar;


DicomScene::DicomScene(gdcm::Image *gdcmImage) {
	this->gdcmImage = gdcmImage;

	auto qimg = ImgConvert::gdcm2Qt(*gdcmImage);
	auto pix = QPixmap::fromImage(*qimg);
	addPixmap(pix);

	text11 = addText("text11");
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

void DicomScene::positTxts() {
	text11.setP
}

void DicomScene::resize(int w, int h) {
	maxWidth = w;
	maxHeight = h;
}
