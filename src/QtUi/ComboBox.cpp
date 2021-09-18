//
// Created by adam on 18.09.2021.
//

#include "ComboBox.hpp"

using namespace QtUi;

ComboBox::ComboBox(QWidget *parent) : QComboBox(parent) {

    scrollAreaFix();
}

void ComboBox::wheelEvent(QWheelEvent *event) {

    if (is_scroll_area_fix and not hasFocus()) {
        event->ignore();
    } else {
        QComboBox::wheelEvent(event);
    }

}

void ComboBox::scrollAreaFix() {

    is_scroll_area_fix = true;
    setFocusPolicy(Qt::StrongFocus);
}
