//
// Created by adam on 20.05.2021.
//

#pragma once

#include "SokarUi.hpp"

#include <QWidget>
#include <SokarAlg/DicomVolume.hpp>
#include <SokarAlg/VolumeSegmentator.hpp>

namespace Ui {
	class SokarSegmentationPipelineEditor;
}

namespace SokarUi {

	class SegmentationPipelineEditor : public QWidget {
	Q_OBJECT
		Ui::SokarSegmentationPipelineEditor *ui;
		QSharedPointer<SokarAlg::SegmentationPipeline> pipeline;

		void setupUi();

	public:
		explicit SegmentationPipelineEditor(QWidget *parent = nullptr);
		~SegmentationPipelineEditor() override;

		const QSharedPointer<SokarAlg::SegmentationPipeline> &getPipeline() const;
	};
}
