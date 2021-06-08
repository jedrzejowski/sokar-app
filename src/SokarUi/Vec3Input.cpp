//
// Created by adam on 08.06.2021.
//

#include "Vec3Input.hpp"
#include "ui_Vec3Input.h"

using namespace SokarUi;

Vec3Input::Vec3Input(QWidget *parent)
		: QWidget(parent),
		  ui(new Ui::Vec3Input()) {
	ui->setupUi(this);

	QObject::connect(ui->xBox, qOverload<qreal>(&QDoubleSpinBox::valueChanged), [this]() { emitChange(); });
	QObject::connect(ui->yBox, qOverload<qreal>(&QDoubleSpinBox::valueChanged), [this]() { emitChange(); });
	QObject::connect(ui->zBox, qOverload<qreal>(&QDoubleSpinBox::valueChanged), [this]() { emitChange(); });
}

Vec3Input::~Vec3Input() {
	delete ui;
}

glm::vec3 Vec3Input::getValue() const {
	return glm::vec3(
			ui->xBox->value(),
			ui->yBox->value(),
			ui->zBox->value()
	);
}

void Vec3Input::setValue(const glm::vec3 &vector) {
	haltEmtChange = true;
	ui->xBox->setValue(vector.x);
	ui->yBox->setValue(vector.y);
	ui->zBox->setValue(vector.z);
	haltEmtChange = false;
}


void Vec3Input::emitChange() {
	if (haltEmtChange) return;
	emit valueChanged(getValue());
}
