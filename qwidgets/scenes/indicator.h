#pragma once

#include <QObject>
#include <QGraphicsObject>

#include "_classdef.h"

class Sokar::SceneIndicator : public QObject{
Q_OBJECT
private:
	std::vector<QGraphicsObject *> items;

public:

	SceneIndicator(QObject *parent = nullptr);

	~SceneIndicator();

	const std::vector<QGraphicsObject *> &allItems();

	size_t itemsCount();

protected:
	void addItem(QGraphicsObject *item);

	bool removeItem(QGraphicsObject *item);
};


