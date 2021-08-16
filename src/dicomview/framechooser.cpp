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
}

FrameChooser::~FrameChooser() {

    delete ui;
}

void FrameChooser::setSceneSet(DicomSceneSet *sceneSet) {

    this->sceneSet = sceneSet;
    this->sceneSequence = sceneSet->getSceneSequence();

    if (sceneSequence->size() == 1) {
        this->hide();
        return;
    }

    for (auto &scene : sceneSet->getScenesVector()) {

        auto avatar = scene->getAvatar();
        avatar->updateSize(20);

        layout->addWidget(avatar);

        avatarsVector << avatar;
        avatarsHash[avatar->getScene()] = avatar;

        connect(this, &FrameChooser::resizeAvatars, avatar, &SokarScene::Avatar::updateSize);
        connect(avatar, &SokarScene::Avatar::clicked, this, &FrameChooser::onAvatarClicked);
    }

    if (!avatarsVector.empty())
        onAvatarClicked(avatarsVector[0]);

    updateAvatars();
}

void FrameChooser::resizeEvent(QResizeEvent *event) {

    updateAvatars();
    QWidget::resizeEvent(event);
}

void FrameChooser::onAvatarClicked(SokarScene::Avatar *avatar) {

    currentAvatar = avatar;
    //TODO zrobić to
//	emit setStep(sceneSequence.indexOf(avatar->getScene()));
}

void FrameChooser::updateAvatars() {

    auto dim = this->contentsRect().width() - ui->scrollArea->verticalScrollBar()->width();
    emit resizeAvatars(dim);
}

void FrameChooser::moveTo(int i) {

    i %= avatarsVector.size();
    onAvatarClicked(avatarsVector[i]);
}

void FrameChooser::moveTo(SokarScene::DicomScene *scene) {

    if (avatarsHash.contains(scene))
        moveTo(avatarsVector.indexOf(avatarsHash[scene]));
}

void FrameChooser::stepChanged(const Step *step) {

}
