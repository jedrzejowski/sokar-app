#include "framechooser.h"

using namespace Sokar;

FrameChooser::FrameChooser(QWidget *parent) : QScrollArea(parent) {

	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	layout = new QBoxLayout(QBoxLayout::Direction::TopToBottom);
	layout->setSpacing(0);
	layout->setMargin(0);

	content = new QWidget;
	content->setLayout(layout);

	setWidget(content);
}

void FrameChooser::addSceneSet(DicomSceneSet *sceneSet) {

	for (auto &scene : sceneSet->getVector())
		addScene(scene);

	this->dumpObjectTree();
}


void FrameChooser::addScene(DicomScene *scene) {
	qDebug("FrameChooser::addScene");
	auto widget = scene->getAvatar();
	layout->addWidget(widget);

}

