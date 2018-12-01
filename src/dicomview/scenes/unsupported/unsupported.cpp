#include "unsupported.h"

using namespace Sokar;

Unsupported::Scene::Scene(SceneParams &sceneParams) : DicomScene(sceneParams) {

	text22 = addText("Unsupported");
	text22->setDefaultTextColor(QColor("white"));
}

Unsupported::Scene::~Scene() {

}

bool Sokar::Unsupported::Scene::generatePixmap() {
	if (!pixmap.isNull())
		return false;

	auto img = QImage(1, 1, QImage::Format_RGB888);
	img.fill(Qt::black);
	pixmap = QPixmap::fromImage(img);
	return true;
}

void Unsupported::Scene::reposItems() {
	DicomScene::reposItems();

	text22->setPos((this->width() - text22->document()->size().width()) / 2,
				   (this->height() - text22->document()->size().height()) / 2);
}
