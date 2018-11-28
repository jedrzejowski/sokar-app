#pragma once

#include <QtCore>
#include <QtWidgets>

#include "dicomview.h"

namespace Sokar {
	class DicomToolBar : public QToolBar {
	Q_OBJECT
	private:
		DicomView *dicomView;

	public:
		explicit DicomToolBar(QWidget *parent);
	private:
		void initActions();
	};
}


