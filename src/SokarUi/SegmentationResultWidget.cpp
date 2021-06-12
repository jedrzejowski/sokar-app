//
// Created by adam on 07.06.2021.
//

#include <QtConcurrent/QtConcurrent>
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
	QObject::connect(ui->saveButton, &QPushButton::clicked, this, &SegmentationResultWidget::saveToWavefrontObjFile);
}

SokarUi::SegmentationResultWidget::~SegmentationResultWidget() {
	delete ui;
}

void SegmentationResultWidget::saveToWavefrontObjFile() {

	auto fileName = QFileDialog::getSaveFileName(
			this,
			tr("Save Wavefront OBJ"), "",
			tr("Wavefront (*.obj);;All Files (*)"));


	QtConcurrent::run(
			[
					this,
					fileName,
					mesh = result->originalMesh
			]() {
				QFile file(fileName);

				if (!file.open(QIODevice::WriteOnly)) {
					QMessageBox::information(
							this, tr("Unable to open file"),
							file.errorString());
					return;
				}

				auto wavefront = SokarLib::WavefrontObjBuilder();
				mesh->dump2wavefront(wavefront);
				wavefront.save2file(file);
			});
}
