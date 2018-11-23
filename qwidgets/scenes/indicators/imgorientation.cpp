#include <QTextDocument>
#include <sokar/dicomtags.h>

#include "imgorientation.h"

using namespace Sokar;

QString ImageOrientationIndicator::Left = "L";
QString ImageOrientationIndicator::Right = "R";
QString ImageOrientationIndicator::Anterior = "A";
QString ImageOrientationIndicator::Posterior = "P";
QString ImageOrientationIndicator::Feet = "F";
QString ImageOrientationIndicator::Head = "H";

ImageOrientationIndicator::ImageOrientationIndicator() {
	leftText = new QGraphicsTextItem();
	leftText->setDefaultTextColor(defaultColor);
	leftText->hide();
	addToGroup(leftText);

	topText = new QGraphicsTextItem();
	topText->setDefaultTextColor(defaultColor);
	topText->hide();
	addToGroup(topText);

	bottomText = new QGraphicsTextItem();
	bottomText->setDefaultTextColor(defaultColor);
	bottomText->hide();
	addToGroup(bottomText);

	rightText = new QGraphicsTextItem();
	rightText->setDefaultTextColor(defaultColor);
	rightText->hide();
	addToGroup(rightText);
}

void ImageOrientationIndicator::setOrientation(QString orient) {
	auto orientStrList = orient.split(gdcm::StringSplitter);

	if (orientStrList.size() != 6) return;

	// orientStrList = [Xx,Xy,Xz,Yx,Yy,Yz]

//	if(orientStrList[0] == '1') {
//		topText->setPlainText();
//	}

	topText->setPlainText("T");
	bottomText->setPlainText("B");
	rightText->setPlainText("R");
	leftText->setPlainText("L");

	topText->show();
	bottomText->show();
	rightText->show();
	leftText->show();
}

void ImageOrientationIndicator::setAngle(qreal angle) {

}

void ImageOrientationIndicator::reposition() {
	topText->setPos(0, (scene()->width() - topText->document()->size().width()) / 2);

	leftText->setPos((scene()->height() - topText->document()->size().height()) / 2, 0);
}


