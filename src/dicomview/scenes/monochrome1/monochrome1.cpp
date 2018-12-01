#include "monochrome1.h"

using namespace Sokar;

Monochrome1::Scene::Scene(Sokar::SceneParams &sceneParams) :
		Sokar::Monochrome2::Scene(sceneParams) {

	imgWindow->setInversed(true);
	reloadPixmap();
}
