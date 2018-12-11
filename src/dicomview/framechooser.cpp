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
	connect(ui->playBtn, &QToolButton::clicked, this, &FrameChooser::timerToggle);
	connect(ui->fpsBox, &QSpinBox::valueChanged, this, &FrameChooser::timerUpdate);
	connect(&qTimer, &QTimer::timeout, this, &FrameChooser::timerStep);
}

FrameChooser::~FrameChooser() {
	delete ui;
}

void FrameChooser::setSceneSet(DicomSceneSet *sceneSet) {

	this->sceneSet = sceneSet;

	for (auto &scene : sceneSet->getVector()) {

		auto avatar = scene->getAvatar();
		avatar->updateSize(20);

		layout->addWidget(avatar);
		avatars << avatar;

		connect(this, &FrameChooser::resizeAvatars, avatar, &SceneAvatar::updateSize);
		connect(avatar, &SceneAvatar::clicked, this, &FrameChooser::onAvatarClicked);
	}

	if (avatars.size() > 0)
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

void FrameChooser::initTimer() {
	qreal frameTime = sceneSet->getFrameTime();

	if (frameTime == 0) return;

	qreal fps = 1000 / frameTime;

	ui->fpsBox->setValue(int(fps));
	qTimer.setInterval(int(frameTime));
	timerToggle();
}

void FrameChooser::timerToggle() {
	if (qTimer.isActive()) {
		qTimer.stop();
	} else {
		qTimer.start();
	}

	ui->nextBtn->setDisabled(qTimer.isActive());
	ui->prevBtn->setDisabled(qTimer.isActive());
}

void FrameChooser::timerUpdate() {
	qTimer.setInterval(1000 / ui->fpsBox->value());
}

void FrameChooser::timerStep() {
	this->moveNext();
}

