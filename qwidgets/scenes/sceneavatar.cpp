#include "sceneavatar.h"
#include "dicomscene.h"

using namespace Sokar;

SceneAvatar::SceneAvatar(DicomScene *scene) :
		QWidget(),
		scene(scene) {

	setAutoFillBackground(true);

	qPalette = QWidget::palette();
	qPalette.setBrush(QPalette::Background, scene->getIcon());
	QWidget::setPalette(qPalette);

}

void SceneAvatar::updateSize(int width) {

	auto icon = scene->getIcon().scaledToWidth(width);

	qPalette.setBrush(QPalette::Background, icon);
	QWidget::setPalette(qPalette);

	setFixedSize(icon.width(), icon.height());
}

void SceneAvatar::mouseReleaseEvent(QMouseEvent *event) {

	if (event->button() == Qt::LeftButton) {
		event->accept();
		emit selectScene(this);
	}

	QWidget::mouseReleaseEvent(event);
}

void SceneAvatar::contextMenuEvent(QContextMenuEvent *event) {
	QWidget::contextMenuEvent(event);
}