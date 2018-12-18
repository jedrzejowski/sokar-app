#include "modality.h"

using namespace Sokar;


ModalityIndicator::ModalityIndicator(DataConverter &dataConverter)
		: SceneIndicator(dataConverter) {

	text = new QGraphicsTextItem();

	genText();
	text->setHtml(lines.join("<br>"));

	addToGroup(text);
}

void ModalityIndicator::genText() {

	const static gdcm::Tag
			TagModality(0x0008, 0x0060);

	auto modality = dataConverter.toString(TagModality);

	if (modality == "US")
		return genTextUS();




}

void ModalityIndicator::genTextUS() {

}
