#pragma once

#include <QtCore>
#include <QtWidgets>

#include "SokarScene.hpp"
#include "Scene.hpp"
#include "SokarDicom/DataConventer.hpp"

namespace SokarScene {

    class Indicator : public QGraphicsItemGroup {
    private:
        qreal offsetRight = 0, offsetTop = 0, offsetLeft = 0, offsetBottom = 0;
        Indicator *offsetRightParent = nullptr, *offsetTopParent = nullptr,
                *offsetLeftParent = nullptr, *offsetBottomParent = nullptr;
    protected:
        QColor defaultColor;
        QPen defaultPen;
        SokarDicom::DataConverter &dataConverter;

        QGraphicsLineItem *newLine(qreal x1, qreal y1, qreal x2, qreal y2);
        QGraphicsTextItem *newText(const QString &string = "");

        static QString wrapAsHtml(QStringList lines, bool right = false);

    public:
        Indicator(SokarDicom::DataConverter &dataConverter);

        virtual void reposition() = 0;

        virtual bool isAlive() = 0;

        //region Getters & Setters

        qreal getOffsetRight() const;
        void setOffsetRight(qreal offsetRight);
        qreal getOffsetTop() const;
        void setOffsetTop(qreal offsetTop);
        qreal getOffsetLeft() const;
        void setOffsetLeft(qreal offsetLeft);
        qreal getOffsetBottom() const;
        void setOffsetBottom(qreal offsetBottom);

        virtual qreal getTopSpace();
        virtual qreal getBottomSpace();
        virtual qreal getRightSpace();
        virtual qreal getLeftSpace();

        Indicator *getOffsetRightParent() const;
        void setOffsetRightParent(Indicator *offsetRightParent);
        Indicator *getOffsetTopParent() const;
        void setOffsetTopParent(Indicator *offsetTopParent);
        Indicator *getOffsetLeftParent() const;
        void setOffsetLeftParent(Indicator *offsetLeftParent);
        Indicator *getOffsetBottomParent() const;
        void setOffsetBottomParent(Indicator *offsetBottomParent);

        //endregion


    };

}