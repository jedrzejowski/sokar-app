#pragma once

#include <gdcmDataSet.h>

#include "../_base.h"

namespace Sokar {

	class ModalityIndicator : public SceneIndicator {
	protected:
		const gdcm::DataSet &dataset;
		QGraphicsTextItem *text;

		virtual QString genText() = 0;

	public:
		explicit ModalityIndicator(const gdcm::DataSet &dataset);

		virtual QString name() = 0;

		static ModalityIndicator *createFor(const gdcm::DataSet &dataset);

	public slots:
		void update();
	};

}