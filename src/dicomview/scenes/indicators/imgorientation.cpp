#include <QTextDocument>
#include "sokar/gdcmSokar.h"

#include "imgorientation.h"

using namespace Sokar;

struct {
	//http://dicomiseasy.blogspot.com/2013/06/getting-oriented-using-image-plane.html?m=1
	QVector4D
			Right = QVector4D(-1, 0, 0, 1),
			Left = QVector4D(+1, 0, 0, 1),
			Head = QVector4D(0, 0, +1, 1),
			Feet = QVector4D(0, 0, -1, 1),
			Anterior = QVector4D(0, -1, 0, 1),
			Posterior = QVector4D(0, +1, 0, 1);
} PatientPoint;

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

	{
		// orientStrList = [Xx,Xy,Xz,Yx,Yy,Yz]
		imgMatrix(0, 0) = orientStrList[0].toFloat();
		imgMatrix(1, 0) = orientStrList[1].toFloat();
		imgMatrix(2, 0) = orientStrList[2].toFloat();
		imgMatrix(0, 1) = orientStrList[3].toFloat();
		imgMatrix(1, 1) = orientStrList[4].toFloat();
		imgMatrix(2, 1) = orientStrList[5].toFloat();

		/**
		 * https://dicom.innolitics.com/ciods/ct-image/image-plane/00200037
		 *
		 * PatientPosition = imgMatrix * ScenePosition
		 * imgMatrix^-1 * PatientPosition = imgMatrix^-1 * imgMatrix * ScenePosition
		 * imgMatrix^-1 * PatientPosition = ScenePosition
		 * ScenePosition = imgMatrix^-1 * PatientPosition
		 */

		auto temp = imgMatrix.inverted();

		scenePosition.anterior = temp * PatientPoint.Anterior;
		scenePosition.posterior = temp * PatientPoint.Posterior;
		scenePosition.left = temp * PatientPoint.Left;
		scenePosition.right = temp * PatientPoint.Right;
		scenePosition.head = temp * PatientPoint.Head;
		scenePosition.feet = temp * PatientPoint.Feet;
	}

	topText->show();
	bottomText->show();
	rightText->show();
	leftText->show();

	update();
}

void ImageOrientationIndicator::reposition() {

	if (scene() == nullptr)
		return;

	leftText->setPos(0, (scene()->height() - leftText->document()->size().height()) / 2);

	topText->setPos((scene()->width() - topText->document()->size().width()) / 2, 0);

	rightText->setPos(
			scene()->width() - rightText->document()->size().width(),
			(scene()->height() - rightText->document()->size().height()) / 2);

	bottomText->setPos(
			(scene()->width() - bottomText->document()->size().width()) / 2,
			scene()->height() - bottomText->document()->size().height());
}

void ImageOrientationIndicator::setRotateTransform(QTransform &rotateTransform) {
	ImageOrientationIndicator::rotateTransform = QMatrix4x4(rotateTransform);
	update();
}

int sceneAngle(const QVector4D &one) {
	auto a = std::atan2(one[1], one[0]);
	a += (M_PI / 8) + M_PI;
	a /= M_PI_4;
	return int(std::floor(a)) % 8;
}

bool isNotCenter(const QVector4D &one) {
	return not(one[0] == 0.0 and one[1] == 0.0);
}

void ImageOrientationIndicator::update() {

	auto anterior = rotateTransform * scenePosition.anterior;
	auto posterior = rotateTransform * scenePosition.posterior;
	auto left = rotateTransform * scenePosition.left;
	auto right = rotateTransform * scenePosition.right;
	auto head = rotateTransform * scenePosition.head;
	auto feet = rotateTransform * scenePosition.feet;

	QVector<QString> chars(8);
	int a;

	if (isNotCenter(anterior)) {
		a = sceneAngle(anterior);
		chars[a] = QObject::tr("A");
	}

	if (isNotCenter(posterior)) {
		a = sceneAngle(posterior);
		chars[a] = QObject::tr("P");
	}

	if (isNotCenter(left)) {
		a = sceneAngle(left);
		chars[a] = QObject::tr("L");
	}

	if (isNotCenter(right)) {
		a = sceneAngle(right);
		chars[a] = QObject::tr("R");
	}

	if (isNotCenter(head)) {
		a = sceneAngle(head);
		chars[a] = QObject::tr("H");
	}

	if (isNotCenter(feet)) {
		a = sceneAngle(feet);
		chars[a] = QObject::tr("F");
	}

	leftText->setPlainText(chars[7] + chars[0] + chars[1]);
	topText->setPlainText(chars[1] + chars[2] + chars[3]);
	rightText->setPlainText(chars[3] + chars[4] + chars[5]);
	bottomText->setPlainText(chars[7] + chars[6] + chars[5]);

	reposition();
}


