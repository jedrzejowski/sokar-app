//
// Created by adam on 18.09.2021.
//

#pragma once

#include <QDoubleSpinBox>
#include "QtUi.hpp"

namespace QtUi {
    class DoubleSpinBox : public QDoubleSpinBox {
    Q_OBJECT
        bool is_scroll_area_fix = false;
    public:

        explicit DoubleSpinBox(QWidget *parent = nullptr);

        void scrollAreaFix();

    protected:

        void wheelEvent(QWheelEvent *event) override;
    };
}


