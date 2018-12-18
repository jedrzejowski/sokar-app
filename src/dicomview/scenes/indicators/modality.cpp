#include "modality.h"

using namespace Sokar;


ModalityIndicator::ModalityIndicator(DataConverter &dataConverter)
		: SceneIndicator(dataConverter) {

	text = new QGraphicsTextItem();

	genText();

	text->setHtml(wrapAsHtml(lines));

	addToGroup(text);
}

void ModalityIndicator::genText() {

	const static gdcm::Tag
			TagModality(0x0008, 0x0060),
			TagSeriesNumber(0x0020, 0x0011),
			TagInstanceNumber(0x0020, 0x0013),
			TagSliceThickness(0x0018, 0x0050),
			TagSliceLocation(0x0020, 0x1041);


	auto toStr = std::bind(&DataConverter::toString, &dataConverter, std::placeholders::_1);

	const static QString millimeter = QObject::tr("<unit>mm</unit>");

	auto modality = toStr(TagModality);

	lines << QObject::tr("Modality: <value>%1</value>").arg(modality);

	if (dataConverter.hasTagWithData(TagSeriesNumber))
		lines << QObject::tr("Series: <value>%1</value>").arg(toStr(TagSeriesNumber));

	if (dataConverter.hasTagWithData(TagInstanceNumber))
		lines << QObject::tr("Instance Num: <value>%1</value>").arg(toStr(TagInstanceNumber));
	{
		if (dataConverter.hasTagWithData(TagSliceThickness) and dataConverter.hasTagWithData(TagSliceLocation))
			lines << QObject::tr("Slice thick: <value>%1</value>%3, loc.: <value>%2</value>%3")
					.arg(QString::number(dataConverter.toDS(TagSliceThickness)[0], 'f', 2),
						 QString::number(dataConverter.toDS(TagSliceLocation)[0], 'f', 2),
						 millimeter);
		else if (dataConverter.hasTagWithData(TagSliceThickness) and not dataConverter.hasTagWithData(TagSliceLocation))
			lines << QObject::tr("Slice thickness: <value>%1</value>%2")
					.arg(QString::number(dataConverter.toDS(TagSliceThickness)[0], 'f', 2), millimeter);
		else if (not dataConverter.hasTagWithData(TagSliceThickness) and dataConverter.hasTagWithData(TagSliceLocation))
			lines << QObject::tr("Slice location: <value>%1</value>%2")
					.arg(QString::number(dataConverter.toDS(TagSliceLocation)[0], 'f', 2), millimeter);
	}

	//region Modality Specials

	if (modality == "US")
		return genTextUS();

	//endregion


}

void ModalityIndicator::reposition() {
	text->setPos(0, scene()->height() - text->document()->size().height());
}

void ModalityIndicator::genTextUS() {

}

