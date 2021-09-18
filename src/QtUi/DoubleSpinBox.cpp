//
// Created by adam on 18.09.2021.
//

#include "DoubleSpinBox.hpp"

using namespace QtUi;

DoubleSpinBox::DoubleSpinBox(QWidget *parent) : QDoubleSpinBox(parent) {

    scrollAreaFix();
}

void DoubleSpinBox::wheelEvent(QWheelEvent *event) {

    if (is_scroll_area_fix and not hasFocus()) {
        event->ignore();
    } else {
        QDoubleSpinBox::wheelEvent(event);
    }

}

void DoubleSpinBox::scrollAreaFix() {

    is_scroll_area_fix = true;
    setFocusPolicy(Qt::StrongFocus);
}
