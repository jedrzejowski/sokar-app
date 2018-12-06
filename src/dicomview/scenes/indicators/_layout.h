#pragma once

#include "_base.h"

namespace Sokar {

	class SceneLayout : public SceneIndicator {
	private:
		struct Item{

		};

	public:
		SceneLayout();
		void reposition() override;
	};

}

//w0 = new QWidget();
//w0->setLayout(&layoutH1);
//w0->setFixedSize(200, 200);
//
//w1 = new QPushButton("w1");
//w2 = new QPushButton("w2");
//w3 = new QPushButton("w3");
//
//layoutH1.addWidget(w1);
//layoutH1.addWidget(w2);
//layoutH1.addWidget(w3);
//
////	addWidget(w0);
//
//layoutH1.setDirection(QBoxLayout::RightToLeft);
//
//layoutH1.invalidate();
//layoutH1.activate();
//
//qDebug() << w1->pos() << w2->pos() << w3->pos();