//
// Created by adam on 18.09.2021.
//

#pragma once

#include <QComboBox>
#include "QtUi.hpp"

namespace QtUi {
    class ComboBox : public QComboBox {
    Q_OBJECT
        bool is_scroll_area_fix = false;
    public:

        explicit ComboBox(QWidget *parent = nullptr);

        void scrollAreaFix();

    protected:

        void wheelEvent(QWheelEvent *event) override;
    };
}


