#include "framechooser.h"

using namespace Sokar;

FrameChooser::FrameChooser(QWidget *parent) : QScrollArea(parent) {

	setWidgetResizable(true);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);


	layout = new QVBoxLayout;
	layout->setSpacing(5);
	layout->setMargin(0);
	layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

	content = new QWidget;
	content->setLayout(layout);

	setWidget(content);

}

void FrameChooser::addSceneSet(DicomSceneSet *sceneSet) {

	for (auto &scene : sceneSet->getVector())
		addScene(scene);

	updateAvatars();
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

	updateAvatars();
	QScrollArea::resizeEvent(event);
}

void FrameChooser::onAvatarClicked(SceneAvatar *avatar) {
	curentAvatar = avatar;
	emit selectSceneSignal(avatar->getScene());
}

void FrameChooser::updateAvatars() {
	auto dim = this->contentsRect().width() - this->verticalScrollBar()->width();
	emit resizeAvatars(dim);

	layout->update();
}

void FrameChooser::moveNext() {
	int i = avatars.indexOf(curentAvatar) + 1;
	if (i >= avatars.size()) i = 0;
	onAvatarClicked(avatars[i]);
}

void FrameChooser::movePrev() {
	int i = avatars.indexOf(curentAvatar) - 1;
	if (i < 0) i = avatars.size() - 1;
	onAvatarClicked(avatars[i]);
}

