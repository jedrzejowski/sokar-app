#include "framechooser.h"
#include "ui_framechooser.h"

using namespace Sokar;

FrameChooser::FrameChooser(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::FrameChooser) {
	ui->setupUi(this);

	layout = new QVBoxLayout;
	layout->setMargin(0);
	layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
	ui->avatarContainer->setLayout(layout);

	ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

	connect(ui->nextBtn, &QToolButton::clicked, this, &FrameChooser::moveNext);
	connect(ui->prevBtn, &QToolButton::clicked, this, &FrameChooser::movePrev);
}

FrameChooser::~FrameChooser() {
	delete ui;
}

void FrameChooser::addSceneSet(DicomSceneSet *sceneSet) {

	for (auto &scene : sceneSet->getVector())
		addScene(scene);

	updateAvatars();
}


void FrameChooser::addScene(DicomScene *scene) {

	auto avatar = scene->getAvatar();
	avatar->updateSize(20);

	layout->addWidget(avatar);
	avatars << avatar;

	connect(this, &FrameChooser::resizeAvatars, avatar, &SceneAvatar::updateSize);
	connect(avatar, &SceneAvatar::clicked, this, &FrameChooser::onAvatarClicked);

	if (avatars.size() == 1)
		onAvatarClicked(avatar);
}

void FrameChooser::resizeEvent(QResizeEvent *event) {

	updateAvatars();
	QWidget::resizeEvent(event);
}

void FrameChooser::onAvatarClicked(SceneAvatar *avatar) {
	curentAvatar = avatar;
	emit selectSceneSignal(avatar->getScene());
}

void FrameChooser::updateAvatars() {
	auto dim = this->contentsRect().width() - ui->scrollArea->verticalScrollBar()->width();
	emit resizeAvatars(dim);

//	ui->scrollLayout->update();
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

