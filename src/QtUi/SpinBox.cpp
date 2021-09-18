//
// Created by adam on 18.09.2021.
//

#include "SpinBox.hpp"

using namespace QtUi;

SpinBox::SpinBox(QWidget *parent) : QSpinBox(parent) {

    scrollAreaFix();
}

void SpinBox::wheelEvent(QWheelEvent *event) {

    if (is_scroll_area_fix and not hasFocus()) {
        event->ignore();
    } else {
        QSpinBox::wheelEvent(event);
    }

}

void SpinBox::scrollAreaFix() {

    is_scroll_area_fix = true;
    setFocusPolicy(Qt::StrongFocus);
}
