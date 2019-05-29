#pragma once

#include "_base.h"

namespace Sokar {

	class ModalityIndicator : public SceneIndicator {
	protected:
		QStringList lines;
		QGraphicsTextItem *text;

	public:
		explicit ModalityIndicator(DataConverter &dataConverter);
		void reposition() override;
		bool isAlive() override;
	private:

		inline QString toStr(const gdcm::Tag &tag) {
			return dataConverter.toString(tag);
		}

		inline QString toNumStr(const gdcm::Tag &tag, int precision = 1) {
			return QString::number(dataConverter.toDS(tag)[0], 'f', precision);
		}

		void genText();

		void genTextCT();
		void genTextUS();
		void genTextMR();
		void genTextRT();
	};
}