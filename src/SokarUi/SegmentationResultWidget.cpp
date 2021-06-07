//
// Created by adam on 07.06.2021.
//

#include "SegmentationResultWidget.hpp"
#include "ui_SegmentationResultWidget.h"

using namespace SokarUi;

SegmentationResultWidget::SegmentationResultWidget(
		QWidget *parent,
		const QSharedPointer<const SokarAlg::SegmentationResult> &result
) : QWidget(parent),
	result(result),
	ui(new Ui::SegmentationResultWidget) {
}

SokarUi::SegmentationResultWidget::~SegmentationResultWidget() {
	delete ui;
}
