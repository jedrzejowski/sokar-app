#include "moviebar.h"
#include "dicomview.h"

#include "ui_moviebar.h"

using namespace Sokar;

MovieBar::MovieBar(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::MovieBar) {
	ui->setupUi(this);


	connect(ui->nextBtn, &QToolButton::clicked, [&]() {
		stop();
		sceneSequence->stepForward();
	});

	connect(ui->prevBtn, &QToolButton::clicked, [&]() {
		stop();
		sceneSequence->stepBackward();
	});

	connect(ui->playBtn, &QToolButton::clicked, this, &MovieBar::togglePlaying);

	connect(ui->movieSlider, &QSlider::valueChanged, [&](int value) {
		stop();
		setStep(sceneSequence->operator[](value));
	});

	connect(ui->sweepBtn, &QToolButton::clicked, [&]() {
		sceneSequence->setSweeping(!sceneSequence->isSweeping());
		updateUI();
	});

	connect(&frameTimer, &QTimer::timeout, [&]() {
		if (isRunning()) sceneSequence->step();
	});

	ui->sameWindowBtn->setIcon(QIcon(":/img/ico/window.png"));
	ui->sameTransformBtn->setIcon(QIcon(":/img/ico/rotateRight.png"));

}

MovieBar::~MovieBar() {
	delete ui;
}

void MovieBar::setSceneSet(DicomSceneSet *sceneSet) {
	this->sceneSet = sceneSet;

	sceneSequence = sceneSet->getSceneSequence();
	if (sceneSequence->size() <= 1) {
		this->hide();
		return;
	}

	ui->movieSlider->setMaximum(sceneSequence->size() - 1);

	connect(sceneSequence, &SceneSequence::steped, [&](const Step *step) {
		setStep(step);
		frameTimer.start(int(step->time * ui->speedBox->value()));
		updateUI();
	});

	updateUI();
}

void MovieBar::togglePlaying() {
	if (isRunning()) stop();
	else start();
}


void MovieBar::start() {
	if (isRunning())
		return;

	movieMode = new MovieMode(this);
	movieMode->setOriginScene(getDicomView()->getDicomScene());
	movieMode->setUseSameTranform(ui->sameTransformBtn->isChecked());
	movieMode->setUseSameWindow(ui->sameWindowBtn->isChecked());

	// W przypadku inicjacji, DicomView nie posiada żadnej sceny dlatego bierzemy pierwszą
	if (movieMode->getOriginScene() == nullptr)
		movieMode->setOriginScene(sceneSequence->operator[](0)->scene);

	for (auto &scene : sceneSet->getScenesVector())
		scene->acceptMovieMode(movieMode);

	sceneSequence->step();

	updateUI();
}

void MovieBar::stop() {
	if (! isRunning()) return;
	frameTimer.stop();

	for (auto &scene : sceneSet->getScenesVector()) {
		scene->disableMovieMode();
	}

	delete movieMode;
	movieMode = nullptr;

	// Trzeba jeszcze powiedzieć scenie, aby dostosowała środowisko
	getDicomView()->getDicomScene()->attached();

	updateUI();
}

void MovieBar::updateUI() {

	static QIcon sweepingIcon = QIcon(":/img/ico/dark/sweeping.png");
	static QIcon loopingIcon = QIcon(":/img/ico/dark/looping.png");
	static QIcon IconPlayerStart = QIcon::fromTheme("player_start");
	static QIcon IconPlayerStop = QIcon::fromTheme("player_stop");

	ui->playBtn->setIcon(isRunning() ? IconPlayerStop : IconPlayerStart);

	ui->sweepBtn->setIcon(sceneSequence->isSweeping() ? sweepingIcon : loopingIcon);

	ui->sameTransformBtn->setDisabled(isRunning());
	ui->sameWindowBtn->setDisabled(isRunning());

}

void MovieBar::stepChanged(const Step *step) {

	int i = sceneSequence->indexOf(step);

	ui->movieSlider->blockSignals(true);
	ui->movieSlider->setValue(i);
	ui->movieSlider->blockSignals(false);
}
