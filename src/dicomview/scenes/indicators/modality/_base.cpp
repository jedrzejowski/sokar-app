#include "_base.h"

using namespace Sokar;

ModalityIndicator *ModalityIndicator::createFor(DataConverter &dataConverter) {

}

ModalityIndicator::ModalityIndicator(DataConverter &dataConverter)
		: SceneIndicator(dataConverter) {

	text = new QGraphicsTextItem();
	text->setHtml(genText());
	addToGroup(text);
}

void ModalityIndicator::update() {

}
