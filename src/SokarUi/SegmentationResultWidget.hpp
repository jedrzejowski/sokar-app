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
	class SegmentationResultWidget : public QWidget {
		Ui::SegmentationResultWidget *ui;
		const QSharedPointer<const SokarAlg::SegmentationResult> result;
	public:
		SegmentationResultWidget(
				const QSharedPointer<const SokarAlg::SegmentationResult> &result,
				QWidget *parent = nullptr);
		virtual ~SegmentationResultWidget();
	signals:
		void clearResult();
	};
}

