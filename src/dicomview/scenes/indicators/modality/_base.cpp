#include "_base.h"

using namespace Sokar;

ModalityIndicator *ModalityIndicator::createFor(const gdcm::DataSet &dataset) {

}

ModalityIndicator::ModalityIndicator(const gdcm::DataSet &dataset)
		: dataset(dataset) {

	text = new QGraphicsTextItem();
	text->setHtml(genText());
	addToGroup(text);
}

void ModalityIndicator::update() {

}
