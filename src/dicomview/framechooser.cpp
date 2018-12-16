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
		avatars << avatar;

		connect(this, &FrameChooser::resizeAvatars, avatar, &SceneAvatar::updateSize);
		connect(avatar, &SceneAvatar::clicked, this, &FrameChooser::onAvatarClicked);
	}

	if (!avatars.empty())
		onAvatarClicked(avatars[0]);

	updateAvatars();
	initTimer();
}

void FrameChooser::resizeEvent(QResizeEvent *event) {

	updateAvatars();
	QWidget::resizeEvent(event);
}

void FrameChooser::onAvatarClicked(SceneAvatar *avatar) {
	curentAvatar = avatar;
	timerStop();
	emit selectSceneSignal(avatar->getScene());
}

void FrameChooser::updateAvatars() {
	auto dim = this->contentsRect().width() - ui->scrollArea->verticalScrollBar()->width();
	emit resizeAvatars(dim);
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

void FrameChooser::moveTo(int i) {
	i %= avatars.size();
	onAvatarClicked(avatars[i]);
}

void FrameChooser::initTimer() {
	frameSequence = sceneSet->getFrameSequence();

	if (frameSequence->size() == 0) return;

	connect(&frameTimer, &QTimer::timeout, this, [&]() {
		frameSequence->step();
	});

	connect(frameSequence, &CommandSequence::onSleep, this, [&](quint64 howlong) {
		frameTimer.start(int(howlong * ui->speedBox->value()));
		updateTimerUI();
	});

	connect(frameSequence, &CommandSequence::onGoTo, this, [&](quint64 to) {
		moveTo(int(to));
		frameSequence->step();
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