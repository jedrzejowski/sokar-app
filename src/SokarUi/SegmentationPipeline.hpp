//
// Created by adam on 20.05.2021.
//

#pragma once

#include "_def.h"

#include <QWidget>

namespace Ui {
	class SokarSegmentationPipeline;
}

namespace SokarUi {
	class SegmentationPipeline : public QWidget {
	Q_OBJECT
		Ui::SokarSegmentationPipeline* ui;

		void setupUi();

	public:
		explicit SegmentationPipeline(QWidget *parent = nullptr);
		virtual ~SegmentationPipeline();
	};
}
