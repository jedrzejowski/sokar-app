//
// Created by adam on 08.06.2021.
//

#pragma once

#include "SokarUi.hpp"
#include "SokarGlm.hpp"

namespace Ui {
    class Vec3Input;
}

namespace QtUi {
    class Vec3Input : public QWidget {
    Q_OBJECT
        Ui::Vec3Input *ui;
        bool haltEmtChange = false;
    public:
        explicit Vec3Input(QWidget *parent = nullptr);
        ~Vec3Input() override;

        [[nodiscard]]
        glm::vec3 getValue() const;

    signals:
        void valueChanged(glm::vec3 vec3);

    public slots:
        void setValue(const glm::vec3 &value);
        void setMinimum(const glm::vec3 &value);
        void setMaximum(const glm::vec3 &value);
        void setMinimum(float value);
        void setMaximum(float value);

    private:
        void emitChange();
    };
}



