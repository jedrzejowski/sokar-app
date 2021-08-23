//
// Created by adam on 16.08.2021.
//

#include "MousePosition.hpp"

using namespace SokarScene;

MousePosition::MousePosition(SokarDicom::DataConverter &dataConverter)
        : Indicator(dataConverter) {

    text = newText();
    addToGroup(text);
    text->setHtml("qwe");

    const static gdcm::Tag
            TagPixelSpacing(0x0028, 0x0030),
            TagInstanceNumber(0x0020, 0x0013),
            TagSliceThickness(0x0018, 0x0050);

    if (dataConverter.hasTagWithData(TagPixelSpacing)) {

        auto spacing = dataConverter.toDecimalString(TagPixelSpacing);
        if (spacing.length() == 2) {
            positionScale = QPointF(spacing[1], spacing[0]);
            positionUnit = "mm";
        }
    } else {
        positionUnit = "wks";
    }

    if (dataConverter.hasTagWithData(TagInstanceNumber) and dataConverter.hasTagWithData(TagSliceThickness)) {

        auto instanceNum = dataConverter.toDS(TagInstanceNumber)[0];
        auto sliceThickness = dataConverter.toDS(TagSliceThickness)[0];
        zLine = QString("z: %1 <unit>mm</unit>").arg((instanceNum - 1) * sliceThickness);
    }

}

void MousePosition::reposition() {

    text->setPos(100, 100);
}

bool MousePosition::isAlive() {

    return true;
}

void MousePosition::sceneMouseMoveEvent(QGraphicsSceneMouseEvent *event, QGraphicsPixmapItem *pixmapItem) {

    static const auto mouseOffset = QPointF(10, 10);


    const auto sceneCenter = QPointF(scene()->width() / 2, scene()->height() / 2);
    const auto imagePosition = (event->scenePos() - sceneCenter) * pixmapItem->transform().inverted();

    if (imagePosition.x() < 0 or imagePosition.y() < 0) {
        setVisible(false);
        return;
    }

    const auto imageSize = pixmapItem->pixmap().size();

    if (imagePosition.x() > imageSize.width() or imagePosition.y() > imageSize.height()) {
        setVisible(false);
        return;
    }


    auto realPos = QPointF(imagePosition.x() * positionScale.x(), imagePosition.y() * positionScale.y());

    QStringList lines = {
//            QString("v: %1"),
            QString("x: %1 <unit>%2</unit>").arg(QString::number(realPos.x(), 'f', 1), positionUnit),
            QString("y: %1 <unit>%2</unit>").arg(QString::number(realPos.y(), 'f', 1), positionUnit),
    };

    if (not zLine.isEmpty()) {
        lines << zLine;
    }


    setVisible(true);
    text->setPos(event->scenePos() + mouseOffset);
    text->setHtml(wrapAsHtml(lines));
}
