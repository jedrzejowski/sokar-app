//
// Created by adam on 20.05.2021.
//

#include "SegmentationPipelineEditor.hpp"
#include "ui_SegmentationPipelineEditor.h"

using namespace SokarUi;


SegmentationPipelineEditor::SegmentationPipelineEditor(QWidget *parent)
		: QWidget(parent), ui(new Ui::SokarSegmentationPipelineEditor) {
	setupUi();
}

SegmentationPipelineEditor::~SegmentationPipelineEditor() {
	delete ui;
}

void SegmentationPipelineEditor::setupUi() {
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
