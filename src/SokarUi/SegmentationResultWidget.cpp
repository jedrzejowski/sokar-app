//
// Created by adam on 07.06.2021.
//

#include "SegmentationResultWidget.hpp"
#include "ui_SegmentationResultWidget.h"

using namespace SokarUi;

SegmentationResultWidget::SegmentationResultWidget(
		const QSharedPointer<const SokarAlg::SegmentationResult> &result,
		QWidget *parent
) : QFrame(parent),
	result(result),
	ui(new Ui::SegmentationResultWidget) {
	ui->setupUi(this);

	QObject::connect(ui->deleteButton, &QPushButton::clicked, [this]() { emit deleteResult(); });
	QObject::connect(ui->saveButton, &QPushButton::clicked, this, &SegmentationResultWidget::saveToObjFile);
}

SokarUi::SegmentationResultWidget::~SegmentationResultWidget() {
	delete ui;
}

void SegmentationResultWidget::saveToObjFile() {
	qDebug() << "saving";
}
