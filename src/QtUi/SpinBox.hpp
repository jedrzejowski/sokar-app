//
// Created by adam on 18.09.2021.
//

#pragma once

#include <QSpinBox>
#include "QtUi.hpp"

namespace QtUi {
    class SpinBox : public QSpinBox {
    Q_OBJECT
        bool is_scroll_area_fix = false;
    public:

        explicit SpinBox(QWidget *parent = nullptr);

        void scrollAreaFix();

    protected:

        void wheelEvent(QWheelEvent *event) override;
    };
}


