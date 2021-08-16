#include <QTextDocument>
#include "sokar/gdcmSokar.h"

#include "ImageOrientation.hpp"

using namespace SokarScene;

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

ImageOrientation::ImageOrientation(SokarDicom::DataConverter &dataConverter) : Indicator(dataConverter) {

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

    if (isAlive())
        initData();
}

void ImageOrientation::initData() {

    static gdcm::Tag
            TagImageOrientation(0x0020, 0x0037),
            TagImagePosition(0x0020, 0x0032);

    auto orientVec = dataConverter.toDecimalString(TagImageOrientation);
    auto posVec = dataConverter.toDecimalString(TagImagePosition);

    if (orientVec.size() != 6 || posVec.size() != 3) return;

    {
        // orientVec = [Xx,Xy,Xz,Yx,Yy,Yz]
        imgMatrix(0, 0) = float(orientVec[0]);
        imgMatrix(1, 0) = float(orientVec[1]);
        imgMatrix(2, 0) = float(orientVec[2]);
        imgMatrix(0, 1) = float(orientVec[3]);
        imgMatrix(1, 1) = float(orientVec[4]);
        imgMatrix(2, 1) = float(orientVec[5]);
//		imgMatrix(0, 3) = float(posVec[0]);
//		imgMatrix(1, 3) = float(posVec[1]);
//		imgMatrix(2, 3) = float(posVec[2]);

//		qDebug() << imgMatrix;

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

void ImageOrientation::reposition() {

    if (scene() == nullptr)
        return;

    leftText->setPos(
            0 + getOffsetLeft(),
            (scene()->height() - leftText->document()->size().height()) / 2);

    topText->setPos(
            (scene()->width() - topText->document()->size().width()) / 2,
            0 + getOffsetTop());

    rightText->setPos(
            scene()->width() - rightText->document()->size().width() - getOffsetRight(),
            (scene()->height() - rightText->document()->size().height()) / 2);

    bottomText->setPos(
            (scene()->width() - bottomText->document()->size().width()) / 2,
            scene()->height() - bottomText->document()->size().height() - getOffsetBottom());
}

void ImageOrientation::setRotateTransform(const QTransform &rotateTransform) {

    this->rotateTransform = QMatrix4x4(rotateTransform);
    update();
}

int sceneAngle(const QVector4D &one) {

    auto a = std::atan2(one[1], one[0]);

    a += M_PI;
    a /= M_PI_4;
    a += 0.5;

//	qDebug() << one << std::atan2(one[1], one[0]) << std::atan2(one[1], one[0]) << a << int(std::floor(a)) % 8;

    return int(std::floor(a)) % 8;
}

bool isNotCenter(const QVector4D &one) {

    return !(one[0] == 0.0 && one[1] == 0.0);
}

void ImageOrientation::update() {

    auto anterior = rotateTransform * scenePosition.anterior;
    auto posterior = rotateTransform * scenePosition.posterior;
    auto left = rotateTransform * scenePosition.left;
    auto right = rotateTransform * scenePosition.right;
    auto head = rotateTransform * scenePosition.head;
    auto feet = rotateTransform * scenePosition.feet;

//	qDebug() << anterior << posterior << left << right << head << feet;

    QVector<QString> chars(8);

    if (isNotCenter(anterior))
        chars[sceneAngle(anterior)] += QObject::tr("A");

    if (isNotCenter(posterior))
        chars[sceneAngle(posterior)] += QObject::tr("P");

    if (isNotCenter(left))
        chars[sceneAngle(left)] += QObject::tr("L");

    if (isNotCenter(right))
        chars[sceneAngle(right)] += QObject::tr("R");

    if (isNotCenter(head))
        chars[sceneAngle(head)] += QObject::tr("H");

    if (isNotCenter(feet))
        chars[sceneAngle(feet)] += QObject::tr("F");

    leftText->setPlainText(chars[7] + chars[0] + chars[1]);
    topText->setPlainText(chars[1] + chars[2] + chars[3]);
    rightText->setPlainText(chars[3] + chars[4] + chars[5]);
    bottomText->setPlainText(chars[7] + chars[6] + chars[5]);

    reposition();
}

bool ImageOrientation::isAlive() {

    static gdcm::Tag TagImageOrientationPatient(0x0020, 0x0037);

    return dataConverter.hasTagWithData(TagImageOrientationPatient);
}


