#include <iostream>

#include <QGraphicsSceneMouseEvent>

#include "scene.h"

using namespace Sokar;

Scene::Scene(QObject *parent) : QGraphicsScene(parent) {

	setBackgroundBrush(Qt::black);
}

Scene::~Scene() {
}

void Scene::addIndicator(SceneIndicator *indicator) {
	indicators.push_back(indicator);
	addItem(indicator);
}

void Scene::removeIndicator(SceneIndicator *indicator) {
	for (auto it = indicators.begin(); it != indicators.end(); ++it) {
		if (indicator != *it) continue;

		removeItem(indicator);
		indicators.erase(it);

		return;
	}
}

void Scene::reposItems() {
	for (auto &indicator: indicators) {
		indicator->reposition();
	}
}

SceneIndicator *Scene::findIndicatorByChild(QGraphicsItem *item) {
	if (item == nullptr) return nullptr;

	for (auto &indi : indicators) {

		for (auto &child : indi->childItems()) {

			if (child == item) {

				return indi;
			}
		}
	}

	return nullptr;
}