#include "moviemode.h"

using namespace Sokar;

MovieMode::MovieMode(QObject *parent) {

}

void MovieMode::setOriginScene(SokarScene::DicomScene *orginScene) {

    MovieMode::originScene = orginScene;
}

void MovieMode::setUseSameWindow(bool useSameWindow) {

    MovieMode::useSameWindow = useSameWindow;
}

void MovieMode::setUseSameTranform(bool useSameTranform) {

    MovieMode::useSameTranform = useSameTranform;
}
