#include "Avatar.hpp"
#include "DicomScene.hpp"

using namespace SokarScene;

Avatar::Avatar(SokarScene::DicomScene *scene) :
        QWidget(),
        scene(scene) {

    setAutoFillBackground(true);

    qPalette = QWidget::palette();
    qPalette.setBrush(QPalette::Window, scene->getIcon());
    QWidget::setPalette(qPalette);

}

void Avatar::updateSize(int width) {

    auto icon = scene->getIcon().scaledToWidth(width);

    qPalette.setBrush(QPalette::Window, icon);
    QWidget::setPalette(qPalette);

    setFixedSize(icon.width(), icon.height());
}

void Avatar::mouseReleaseEvent(QMouseEvent *event) {

    if (event->button() == Qt::LeftButton) {
        event->accept();
        emit clicked(this);
    }

    QWidget::mouseReleaseEvent(event);
}

void Avatar::contextMenuEvent(QContextMenuEvent *event) {

    QWidget::contextMenuEvent(event);

    if (menu.isEmpty()) {
        menu.addAction(new QAction("Pokaż"));
        menu.addAction(new QAction("Odładuj obraz"));
        menu.addAction(new QAction("Odładuj plik"));
        menu.addAction(new QAction("Właściwości"));
    }

    menu.exec(event->globalPos());
}