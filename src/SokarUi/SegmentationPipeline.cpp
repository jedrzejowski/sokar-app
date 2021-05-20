//
// Created by adam on 20.05.2021.
//

#include "SegmentationPipeline.hpp"
#include "ui_SegmentationPipeline.h"

using namespace SokarUi;



SegmentationPipeline::SegmentationPipeline(QWidget *parent)
		: QWidget(parent), ui(new Ui::SokarSegmentationPipeline) {
	setupUi();
}

SegmentationPipeline::~SegmentationPipeline() {
	delete ui;
}

void SegmentationPipeline::setupUi() {
	ui->setupUi(this);

	QObject::connect(
			ui->interpolationCombo,
			qOverload<int>(&QComboBox::currentIndexChanged),
			[=](auto index) {

//				switch(index){
//					case 0:
//					case 1:
//					default:
//				};
			}
	);

	QObject::connect(
			ui->segmentationComboBox,
			qOverload<int>(&QComboBox::currentIndexChanged),
			[=](auto index) {
//				switch(index){
//					case 0:
//					case 1:
//					default:
//				};
			}
	);
}
