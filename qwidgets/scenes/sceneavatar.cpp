#include "sceneavatar.h"
#include "dicomscene.h"

using namespace Sokar;

SceneAvatar::SceneAvatar(DicomScene *scene) :
		QWidget(),
		scene(*scene){

	setAutoFillBackground(true);

	qPalette = palette();
	qPalette.setBrush(QPalette::Background, scene->getIcon());
	setPalette(qPalette);

	resize(50,50);
}

void SceneAvatar::resizeEvent(QResizeEvent *event) {

	resize(50,50);
}
