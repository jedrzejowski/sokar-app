//
// Created by adam on 07.06.2021.
//

#pragma once

#include "SokarUi.hpp"
#include "SokarAlg/SegmentationPipeline.hpp"

namespace Ui {
	class SegmentationResultWidget;
}

namespace SokarUi {
	class SegmentationResultWidget : public QFrame {
	Q_OBJECT
		Ui::SegmentationResultWidget *ui;
		SokarAlg::SegmentationResultCPtr result;
	public:

		explicit SegmentationResultWidget(
				const SokarAlg::SegmentationResultCPtr &result,
				QWidget *parent = nullptr);
		~SegmentationResultWidget() override;

	public slots:
		void saveToWavefrontObjFile();

	signals:
		void deleteResult();
	};
}

