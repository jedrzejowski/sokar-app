#include "Indicator.hpp"

using namespace SokarScene;

Indicator::Indicator(SokarDicom::DataConverter &dataConverter) :
        defaultColor(255, 255, 255),
        defaultPen(defaultColor),
        dataConverter(dataConverter) {
}

qreal Indicator::getTopSpace() { return 0; }

qreal Indicator::getBottomSpace() { return 0; }

qreal Indicator::getRightSpace() { return 0; }

qreal Indicator::getLeftSpace() { return 0; }

qreal Indicator::getOffsetRight() const {

    auto out = offsetRight;
    if (offsetRightParent != nullptr)
        out += offsetRightParent->getOffsetTop() + offsetRightParent->getRightSpace();
    return out;
}

void Indicator::setOffsetRight(qreal offsetRight) {

    Indicator::offsetRight = offsetRight;
}

qreal Indicator::getOffsetTop() const {

    auto out = offsetTop;
    if (offsetTopParent != nullptr)
        out += offsetTopParent->getOffsetTop() + offsetTopParent->getTopSpace();
    return out;
}

void Indicator::setOffsetTop(qreal offsetTop) {

    Indicator::offsetTop = offsetTop;
}

qreal Indicator::getOffsetLeft() const {

    auto out = offsetLeft;
    if (offsetLeftParent != nullptr)
        out += offsetLeftParent->getOffsetTop() + offsetLeftParent->getLeftSpace();
    return out;
}

void Indicator::setOffsetLeft(qreal offsetLeft) {

    Indicator::offsetLeft = offsetLeft;
}

qreal Indicator::getOffsetBottom() const {

    auto out = offsetBottom;
    if (offsetBottomParent != nullptr)
        out += offsetBottomParent->getOffsetTop() + offsetBottomParent->getBottomSpace();
    return out;
}

void Indicator::setOffsetBottom(qreal offsetBottom) {

    Indicator::offsetBottom = offsetBottom;
}

Indicator *Indicator::getOffsetRightParent() const {

    return offsetRightParent;
}

void Indicator::setOffsetRightParent(Indicator *offsetRightParent) {

    Indicator::offsetRightParent = offsetRightParent;
}

Indicator *Indicator::getOffsetTopParent() const {

    return offsetTopParent;
}

void Indicator::setOffsetTopParent(Indicator *offsetTopParent) {

    Indicator::offsetTopParent = offsetTopParent;
}

Indicator *Indicator::getOffsetLeftParent() const {

    return offsetLeftParent;
}

void Indicator::setOffsetLeftParent(Indicator *offsetLeftParent) {

    Indicator::offsetLeftParent = offsetLeftParent;
}

Indicator *Indicator::getOffsetBottomParent() const {

    return offsetBottomParent;
}

void Indicator::setOffsetBottomParent(Indicator *offsetBottomParent) {

    Indicator::offsetBottomParent = offsetBottomParent;
}

QGraphicsLineItem *Indicator::newLine(qreal x1, qreal y1, qreal x2, qreal y2) {

    auto line = new QGraphicsLineItem(x1, y1, x2, y2, this);
    line->setPen(defaultPen);
    return line;
}

QGraphicsTextItem *Indicator::newText(const QString &string) {

    auto text = new QGraphicsTextItem(this);
    text->setDefaultTextColor(defaultColor);
    text->setHtml(string);
    text->adjustSize();
    return text;
}

QString Indicator::wrapAsHtml(QStringList lines, bool right) {


    const static QString htmlHead = QObject::tr(
            "<html>"
            "<head>"
            "<style>"
            "body{font-size:medium;background-color:rgba(0,0,0,0.5);}"
            "unit{font-style:italic;font-size:small;}"
            "value{font-weight:500;}"
            "div{white-space: nowrap;}"
            "</style>"
            "</head>"
            "<body>");

    QString align = QString("align='%1'").arg(
            right ? "right" : "left"
    );

    for (auto &line : lines)
        line = QString("<div class='line' %1>%2</div>").arg(
                align,
                line
        );

    return htmlHead + lines.join("") + "</body></html>";
}
