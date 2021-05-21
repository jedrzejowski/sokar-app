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

	struct SegmentationPipelineData {
	};

	class SegmentationPipelineEditor : public QWidget {
	Q_OBJECT
		Ui::SokarSegmentationPipelineEditor *ui;

		void setupUi();

	public:
		explicit SegmentationPipelineEditor(QWidget *parent = nullptr);
		virtual ~SegmentationPipelineEditor();
	};
}
