#include "moviemode.h"

using namespace Sokar;

MovieMode::MovieMode(QObject *parent) {

}

void MovieMode::setOriginScene(DicomScene *orginScene) {
	MovieMode::originScene = orginScene;
}
