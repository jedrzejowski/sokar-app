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
	connect(ui->playBtn, &QToolButton::clicked, this, &FrameChooser::toggleMovieMode);

	connect(ui->sweepBtn, &QToolButton::clicked, this, [&]() {
		frameSequence->setSweeping(!frameSequence->isSweeping());
		updateMovieModeUI();
	});
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

	setupFrameMovieMode();
}

void FrameChooser::resizeEvent(QResizeEvent *event) {

	updateAvatars();
	QWidget::resizeEvent(event);
}

void FrameChooser::onAvatarClicked(SceneAvatar *avatar) {
	stopMovieMode();

	currentAvatar = avatar;

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

void FrameChooser::toggleMovieMode() {
	if (isMovieMode()) stopMovieMode();
	else startMovieMode();
}


void FrameChooser::setupFrameMovieMode() {

	frameSequence = sceneSet->getFrameSequence();
	if (frameSequence->size() <= 1) {
		ui->timerWidget->hide();
		return;
	}

	connect(&frameTimer, &QTimer::timeout, this, [&]() {
		frameSequence->step();
	});

	connect(frameSequence, &SceneSequence::steped, this, [&](const Step &step) {
		moveTo(step.scene);
		frameTimer.start(int(step.time * ui->speedBox->value()));
		updateMovieModeUI();
	});
}

void FrameChooser::startMovieMode() {
	if (isMovieMode())
		return;

	qDebug("startMovieMode");

	movieMode = new MovieMode(this);

	for (auto &avatar : avatarsVector) {

		auto scene = avatar->getScene();

		if (not scene->acceptMovieMode(movieMode)) {
			stopMovieMode();
			return;
		}

		scene->reloadPixmap();
	}

	frameSequence->reset();
	frameSequence->step();

	updateMovieModeUI();
}

void FrameChooser::stopMovieMode() {
	if (not isMovieMode()) return;
	frameTimer.stop();

	for (auto &avatar : avatarsVector) {
		avatar->getScene()->disableMovieMode();
	}
	delete movieMode;
	movieMode = nullptr;
}

void FrameChooser::updateMovieModeUI() {

	static QIcon sweepingIcon = QIcon(":/img/ico/dark/sweeping.png");
	static QIcon loopingIcon = QIcon(":/img/ico/dark/looping.png");
	static QIcon IconPlayerStart = QIcon::fromTheme("player_start");
	static QIcon IconPlayerStop = QIcon::fromTheme("player_stop");

	ui->playBtn->setIcon(frameTimer.isActive() ? IconPlayerStop : IconPlayerStart);

	ui->nextBtn->setDisabled(isMovieMode());
	ui->prevBtn->setDisabled(isMovieMode());

	ui->sweepBtn->setIcon(frameSequence->isSweeping() ? sweepingIcon : loopingIcon);
}
