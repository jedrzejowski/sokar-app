#pragma once

#include <QGraphicsItemGroup>
#include "../_classdef.h"
#include "../scene.h"

class Sokar::SceneIndicator : public QGraphicsItemGroup {
protected:
	QColor defaultColor;
public:
	SceneIndicator();

	virtual void reposition() = 0;

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
};


