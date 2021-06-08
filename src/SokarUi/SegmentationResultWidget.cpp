//
// Created by adam on 07.06.2021.
//

#include "SegmentationResultWidget.hpp"
#include "ui_SegmentationResultWidget.h"

using namespace SokarUi;

SegmentationResultWidget::SegmentationResultWidget(
		const QSharedPointer<const SokarAlg::SegmentationResult> &result,
		QWidget *parent
) : QWidget(parent),
	result(result),
	ui(new Ui::SegmentationResultWidget) {
	ui->setupUi(this);
}

SokarUi::SegmentationResultWidget::~SegmentationResultWidget() {
	delete ui;
}
