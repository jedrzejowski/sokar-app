//
// Created by adam on 08.06.2021.
//

#include "Vec3Input.hpp"
#include "ui_Vec3Input.h"

using namespace QtUi;

Vec3Input::Vec3Input(QWidget *parent)
        : QWidget(parent),
          ui(new Ui::Vec3Input()) {

    ui->setupUi(this);
    ui->xBox->scrollAreaFix();
    ui->yBox->scrollAreaFix();
    ui->zBox->scrollAreaFix();
    ui->label->setVisible(false);

    QObject::connect(ui->xBox, qOverload<qreal>(&QDoubleSpinBox::valueChanged), [this]() { emitChange(); });
    QObject::connect(ui->yBox, qOverload<qreal>(&QDoubleSpinBox::valueChanged), [this]() { emitChange(); });
    QObject::connect(ui->zBox, qOverload<qreal>(&QDoubleSpinBox::valueChanged), [this]() { emitChange(); });
}

Vec3Input::~Vec3Input() {

    delete ui;
}

glm::vec3 Vec3Input::getValue() const {

    return {
            ui->xBox->value(),
            ui->yBox->value(),
            ui->zBox->value()
    };
}

void Vec3Input::setValue(const glm::vec3 &vector) {

    haltEmtChange = true;
    ui->xBox->setValue(vector.x);
    ui->yBox->setValue(vector.y);
    ui->zBox->setValue(vector.z);
    haltEmtChange = false;
}

void Vec3Input::setMinimum(const glm::vec3 &vector) {

    ui->xBox->setMinimum(vector.x);
    ui->yBox->setMinimum(vector.y);
    ui->zBox->setMinimum(vector.z);
}

void Vec3Input::setMaximum(const glm::vec3 &vector) {

    ui->xBox->setMaximum(vector.x);
    ui->yBox->setMaximum(vector.y);
    ui->zBox->setMaximum(vector.z);
}

void Vec3Input::setMinimum(float value) {

    setMinimum({value, value, value});
}

void Vec3Input::setMaximum(float value) {

    setMaximum({value, value, value});
}

void Vec3Input::emitChange() {

    if (haltEmtChange) return;
    emit valueChanged(getValue());
}
