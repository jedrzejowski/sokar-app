#pragma once

#include "_base.h"

namespace Sokar {

	class ModalityIndicator : public SceneIndicator {
	protected:
		QStringList lines;
		QGraphicsTextItem *text;

	public:
		explicit ModalityIndicator(DataConverter &dataConverter);

	private:
	public:
		void reposition() override;
	private:
		void genText();

		void genTextUS();

	};

}