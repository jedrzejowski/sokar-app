
#include "scene.h"

using namespace Sokar;

Scene::Scene(QWidget *parent) : QGraphicsScene(parent) {

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

