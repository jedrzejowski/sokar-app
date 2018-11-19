#include "unsupported.h"

using namespace Sokar;

Unsupported::Scene::Scene(const gdcm::ImageReader &imageReader, Sokar::SceneParams *sceneParams)
		: DicomScene(imageReader, sceneParams) {

	text22 = addText("Unsupported");
	text22->setDefaultTextColor(textColor);
}

Unsupported::Scene::~Scene() {

}

bool Sokar::Unsupported::Scene::genQPixmap() {
	pixmap = QPixmap(0, 0);
	return true;
}

void Unsupported::Scene::reposItems() {
	DicomScene::reposItems();

	text22->setPos((this->width() - text22->document()->size().width()) / 2,
				   (this->height() - text22->document()->size().height()) / 2);
}
