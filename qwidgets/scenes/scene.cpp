
#include "scene.h"

using namespace Sokar;

Scene::Scene(QWidget *parent) : QGraphicsScene(parent) {

}

Scene::~Scene() {

}

void Scene::addIndicator(SceneIndicator *item) {
	indicators.push_back(item);
}

bool Scene::removeIndicator(SceneIndicator *item) {
	for (auto it = indicators.begin(); it != indicators.end(); ++it) {
		indicators.erase(it);
		return true;
	}

	return false;
}
