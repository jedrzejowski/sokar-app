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
		  pipeline(QSharedPointer<SokarAlg::SegmentationPipeline>::create()),
		  ui(new Ui::SokarSegmentationPipelineEditor) {

	setupUi();
}

SegmentationPipelineEditor::~SegmentationPipelineEditor() {
	delete ui;
}

void SegmentationPipelineEditor::setupUi() {
	ui->setupUi(this);


	QObject::connect(
			ui->wokselSize,
			qOverload<double>(&QDoubleSpinBox::valueChanged),
			[=](double value) {
				pipeline->dicomVolume->setCubesPerMM(1.f / float(value));
			});

	QObject::connect(
			ui->interpolationCombo,
			qOverload<int>(&QComboBox::currentIndexChanged),
			[=](auto index) {

				switch (index) {

					case Methods::Nearest:
						pipeline->volumeInterpolator = QSharedPointer<SokarAlg::NearestVolumeInterpolator>::create();
						break;

					case Methods::Linear:
						pipeline->volumeInterpolator = QSharedPointer<SokarAlg::LinearVolumeInterpolator>::create();
						break;

					case Methods::Poly1:
						pipeline->volumeInterpolator = QSharedPointer<SokarAlg::PolynomialVolumeInterpolator1>::create();
						break;

					case Methods::Poly2:
						pipeline->volumeInterpolator = QSharedPointer<SokarAlg::PolynomialVolumeInterpolator2>::create();
						break;

					case Methods::Cubic:
						pipeline->volumeInterpolator = QSharedPointer<SokarAlg::CubicVolumeInterpolator>::create();
						break;

					case Methods::CalumRom:
						pipeline->volumeInterpolator = QSharedPointer<SokarAlg::CubicVolumeInterpolator>::create(true);
						break;

					default:
						throw sokarException("SegmentationPipelineEditor", "unknown index");
				}
			});

	QObject::connect(
			ui->segmentationComboBox,
			qOverload<int>(&QComboBox::currentIndexChanged),
			[=](auto index) {
				switch (index) {

					case Methods::MarchingCubes:
						pipeline->volumeSegmentator = QSharedPointer<SokarAlg::MarchingCubes>::create();
						break;

					case Methods::MarchingTetra:
						pipeline->volumeSegmentator = QSharedPointer<SokarAlg::MarchingCubes>::create();
						break;

					default:
						throw sokarException("SegmentationPipelineEditor", "unknown index");
				}
			});

	QObject::connect(
			ui->useCache,
			qOverload<int>(&QComboBox::currentIndexChanged),
			[=](auto index) {
				pipeline->usingCache = index == 0;
			});

	ui->colorPushButton->setText(pipeline->color.name());
	QObject::connect(
			ui->colorPushButton,
			&QPushButton::clicked,
			[=]() {
				pipeline->color = QColorDialog::getColor(pipeline->color, this, "Wybierz kolor");
				ui->colorPushButton->setText(pipeline->color.name());
			});
}

const QSharedPointer<SokarAlg::SegmentationPipeline> &SegmentationPipelineEditor::getPipeline() const {
	return pipeline;
}
