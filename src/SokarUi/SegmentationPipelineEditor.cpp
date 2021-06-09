//
// Created by adam on 20.05.2021.
//

#include <SokarException.hpp>
#include "SegmentationPipelineEditor.hpp"
#include "ui_SegmentationPipelineEditor.h"
#include "SokarAlg/SegmentationPipeline.hpp"
#include "SokarAlg/MarchingCubes.hpp"

using namespace SokarUi;

enum Methods {
	Nearest = 0,
	Linear = 1,
	Poly1 = 2,
	Poly2 = 3,
	Cubic = 4,
	CalumRom = 5,

	MarchingCubes = 0,
	MarchingTetra = 1,
};


SegmentationPipelineEditor::SegmentationPipelineEditor(QWidget *parent)
		: QWidget(parent),
		  ui(new Ui::SokarSegmentationPipelineEditor) {

	setupUi();
}

SegmentationPipelineEditor::~SegmentationPipelineEditor() {
	delete ui;
}

void SegmentationPipelineEditor::setupUi() {
	ui->setupUi(this);

	setMeshColor(meshColor);
	QObject::connect(
			ui->pickColorButton,
			&QPushButton::clicked,
			[this]() {
				setMeshColor(QColorDialog::getColor(meshColor, this, "Wybierz kolor"));
			});
	QObject::connect(
			ui->randomColorButton,
			&QPushButton::clicked,
			[this]() { randomizeMeshColor(); });
}

QSharedPointer<SokarAlg::SegmentationPipeline> SegmentationPipelineEditor::makePipeline() const {
	auto pipeline = QSharedPointer<SokarAlg::SegmentationPipeline>::create();

	//region interpolation


	QSharedPointer<SokarAlg::VolumeInterpolator> volumeInterpolator = nullptr;

	switch (ui->interpolationCombo->currentIndex()) {

		case Methods::Nearest:
			volumeInterpolator = QSharedPointer<SokarAlg::NearestVolumeInterpolator>::create();
			break;

		case Methods::Linear:
			volumeInterpolator = QSharedPointer<SokarAlg::LinearVolumeInterpolator>::create();
			break;

		case Methods::Poly1:
			volumeInterpolator = QSharedPointer<SokarAlg::PolynomialVolumeInterpolator1>::create();
			break;

		case Methods::Poly2:
			volumeInterpolator = QSharedPointer<SokarAlg::PolynomialVolumeInterpolator2>::create();
			break;

		case Methods::Cubic:
			volumeInterpolator = QSharedPointer<SokarAlg::CubicVolumeInterpolator>::create();
			break;

		case Methods::CalumRom:
			volumeInterpolator = QSharedPointer<SokarAlg::CubicVolumeInterpolator>::create(true);
			break;
	}

	pipeline->setVolumeInterpolator(volumeInterpolator);

	pipeline->getDicomVolume()->setCubesPerMM(float(ui->interpolationWokselSize->value()));
	pipeline->setUseInterpolationCache(ui->cacheInterpolation->isChecked() == 0);

	//endregion

	//region segementation


	QSharedPointer<SokarAlg::VolumeSegmentator> volumeSegmentator = nullptr;

	switch (ui->segmentationAlgorithm->currentIndex()) {

		case Methods::MarchingCubes:
			volumeSegmentator = QSharedPointer<SokarAlg::MarchingCubes>::create();
			break;

		case Methods::MarchingTetra:
			volumeSegmentator = QSharedPointer<SokarAlg::MarchingCubes>::create();
			break;
	}

	qDebug() << volumeSegmentator << ui->segmentationAlgorithm->currentIndex();

	volumeSegmentator->setIsoLevel({
										   static_cast<float>(ui->segmentationTresholdUp->value()),
										   static_cast<float>(ui->segmentationTresholdDown->value()),
								   });
	pipeline->setVolumeSegmentator(volumeSegmentator);

	//endergion

	//region desgin

	pipeline->setColor(meshColor);
	pipeline->setUseEmptyEnv(ui->emptyEnv->isChecked());

	//endregion

	return pipeline;
}

void SegmentationPipelineEditor::randomizeMeshColor() {
	static auto generator = QRandomGenerator();
	auto color = QColor();
	color.setHslF(
			generator.generateDouble(),
			0.5,
			0.5
	);
	setMeshColor(color);
}

void SegmentationPipelineEditor::setMeshColor(const QColor &color) {
	meshColor = color;
	ui->pickColorButton->setText(meshColor.name());
}
