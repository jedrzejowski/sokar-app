#pragma once

#include <QtCore>
#include <QtWidgets>

#include "../../_classdef.h"
#include "../scene.h"

class Sokar::SceneIndicator : public QGraphicsItemGroup {
protected:
	QColor defaultColor;
public:
	SceneIndicator();

	virtual void reposition() = 0;
};


