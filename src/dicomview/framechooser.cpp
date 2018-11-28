#include "framechooser.h"

using namespace Sokar;

FrameChooser::FrameChooser(QWidget *parent) : QScrollArea(parent) {

	setWidgetResizable(true);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);


	layout = new QVBoxLayout;
	layout->setSpacing(0);
	layout->setMargin(0);
	layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

	content = new QWidget;
	content->setLayout(layout);

	setWidget(content);

}

void FrameChooser::addSceneSet(DicomSceneSet *sceneSet) {

	for (auto &scene : sceneSet->getVector())
		addScene(scene);
}


void FrameChooser::addScene(DicomScene *scene) {

	auto avatar = scene->getAvatar();
	layout->addWidget(avatar);

	avatars << avatar;

	connect(this, &FrameChooser::resizeAvatars, avatar, &SceneAvatar::updateSize);
	connect(avatar, &SceneAvatar::clicked, this, &FrameChooser::onAvatarClicked);

	if (avatars.size() == 1)
		onAvatarClicked(avatar);
}

void FrameChooser::resizeEvent(QResizeEvent *event) {

	QScrollArea::resizeEvent(event);

	auto dim = this->contentsRect().width() - this->verticalScrollBar()->width();
	emit resizeAvatars(dim);

	layout->update();
}

void FrameChooser::onAvatarClicked(SceneAvatar *avatar) {
	emit selectSceneSignal(avatar->getScene());
}

