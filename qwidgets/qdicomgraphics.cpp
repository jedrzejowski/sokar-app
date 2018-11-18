#include <iostream>
#include "qdicomgraphics.h"

using namespace Sokar;

QDicomGraphics::QDicomGraphics(QWidget *parent) : QGraphicsView(parent) {

	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

//Aby nie było scrola
void QDicomGraphics::scrollContentsBy(int, int) {}

