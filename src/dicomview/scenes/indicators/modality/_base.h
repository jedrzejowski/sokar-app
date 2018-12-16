#pragma once

#include <gdcmDataSet.h>

#include "../_base.h"

namespace Sokar {

	class ModalityIndicator : public SceneIndicator {
	protected:
		QGraphicsTextItem *text;

		virtual QString genText() {};

	public:
		explicit ModalityIndicator(DataConverter &dataConverter);

		virtual QString name() = 0;

		static ModalityIndicator *createFor(DataConverter &dataConverter);

	public slots:
		void update();
	};

}