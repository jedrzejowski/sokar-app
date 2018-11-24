#include "framechooser.h"

using namespace Sokar;

FrameChooser::FrameChooser(QWidget *parent) : QScrollArea(parent) {

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	layout = new QVBoxLayout;
	layout->setAlignment(Qt::AlignTop);
	layout->setMargin(0);

	content = new QWidget;
	content->setLayout(layout);

	setWidget(content);
}

void FrameChooser::addSceneSet(DicomSceneSet *sceneSet) {

	for (auto &scene : sceneSet->getVector())
		addScene(scene);
}


void FrameChooser::addScene(DicomScene *scene) {
	auto widget = scene->genFrameIcon();
	layout->addWidget(widget);

}

