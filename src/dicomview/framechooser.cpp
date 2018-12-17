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

	connect(ui->nextBtn, SIGNAL(clicked()), this, SLOT(moveNext()));
	connect(ui->prevBtn, SIGNAL(clicked()), this, SLOT(movePrev()));
	connect(ui->playBtn, SIGNAL(clicked()), this, SLOT(timerToggle()));
}

FrameChooser::~FrameChooser() {
	delete ui;
}

void FrameChooser::setSceneSet(DicomSceneSet *sceneSet) {

	this->sceneSet = sceneSet;

	for (auto &scene : sceneSet->getScenesVector()) {

		auto avatar = scene->getAvatar();
		avatar->updateSize(20);

		layout->addWidget(avatar);

		avatarsVector << avatar;
		avatarsHash[avatar->getScene()] = avatar;

		connect(this, &FrameChooser::resizeAvatars, avatar, &SceneAvatar::updateSize);
		connect(avatar, &SceneAvatar::clicked, this, &FrameChooser::onAvatarClicked);
	}

	if (!avatarsVector.empty())
		onAvatarClicked(avatarsVector[0]);

	updateAvatars();
	initTimer();
}

void FrameChooser::resizeEvent(QResizeEvent *event) {

	updateAvatars();
	QWidget::resizeEvent(event);
}

void FrameChooser::onAvatarClicked(SceneAvatar *avatar) {
	currentAvatar = avatar;

	timerStop();
	emit selectSceneSignal(avatar->getScene());
}

void FrameChooser::updateAvatars() {
	auto dim = this->contentsRect().width() - ui->scrollArea->verticalScrollBar()->width();
	emit resizeAvatars(dim);
}

void FrameChooser::moveNext() {
	int i = avatarsVector.indexOf(currentAvatar) + 1;
	if (i >= avatarsVector.size()) i = 0;
	onAvatarClicked(avatarsVector[i]);
}

void FrameChooser::movePrev() {
	int i = avatarsVector.indexOf(currentAvatar) - 1;
	if (i < 0) i = avatarsVector.size() - 1;
	onAvatarClicked(avatarsVector[i]);
}

void FrameChooser::moveTo(int i) {
	i %= avatarsVector.size();
	onAvatarClicked(avatarsVector[i]);
}

void FrameChooser::moveTo(DicomScene *scene) {
	if (avatarsHash.contains(scene))
		moveTo(avatarsVector.indexOf(avatarsHash[scene]));
}

void FrameChooser::initTimer() {
	frameSequence = sceneSet->getFrameSequence();

	if (frameSequence == nullptr || frameSequence->size() < 2) {
		ui->timerWidget->hide();
		return;
	}

	connect(&frameTimer, &QTimer::timeout, this, [&]() {
		frameSequence->step();
	});

	connect(frameSequence, &SceneSequence::steped, this, [&](const Step &step) {
		moveTo(step.scene);
		frameTimer.start(int(step.time * ui->speedBox->value()));
	});

	frameSequence->step();

	updateTimerUI();
}

void FrameChooser::timerToggle() {
	if (frameTimer.isActive()) timerStop();
	else timerStart();
}

void FrameChooser::timerStart() {
	frameTimer.start();
	updateTimerUI();
}

void FrameChooser::timerStop() {
	frameTimer.stop();
	updateTimerUI();
}

void FrameChooser::updateTimerUI() {
	if (frameTimer.isActive()) {
		ui->playBtn->setIcon(QIcon::fromTheme("player_stop"));
	} else {
		ui->playBtn->setIcon(QIcon::fromTheme("player_play"));
	}

	ui->nextBtn->setDisabled(frameTimer.isActive());
	ui->prevBtn->setDisabled(frameTimer.isActive());
}