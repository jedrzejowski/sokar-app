#include "Modality.hpp"

using namespace SokarScene;


Modality::Modality(SokarDicom::DataConverter &dataConverter)
        : Indicator(dataConverter) {

    genText();
    text = newText(wrapAsHtml(lines));
}

void Modality::genText() {

    const static gdcm::Tag
            TagModality(0x0008, 0x0060),
            TagSeriesNumber(0x0020, 0x0011),
            TagInstanceNumber(0x0020, 0x0013),
            TagSliceThickness(0x0018, 0x0050),
            TagSliceLocation(0x0020, 0x1041);

    const static QString millimeter = QObject::tr("<unit>mm</unit>");

    auto modality = toStr(TagModality);

    lines << QObject::tr("Modality: <value>%1</value>").arg(modality);

    if (dataConverter.hasTagWithData(TagSeriesNumber))
        lines << QObject::tr("Series: <value>%1</value>").arg(toNumStr(TagSeriesNumber, 0));

    if (dataConverter.hasTagWithData(TagInstanceNumber))
        lines << QObject::tr("Instance number: <value>%1</value>").arg(toNumStr(TagInstanceNumber, 0));

    {
        if (dataConverter.hasTagWithData(TagSliceThickness) && dataConverter.hasTagWithData(TagSliceLocation))
            lines << QObject::tr("Slice thick: <value>%1</value>%3, loc.: <value>%2</value>%3")
                    .arg(toNumStr(TagSliceThickness, 2),
                         toNumStr(TagSliceLocation, 2),
                         millimeter);
        else if (dataConverter.hasTagWithData(TagSliceThickness) && !dataConverter.hasTagWithData(TagSliceLocation))
            lines << QObject::tr("Slice thickness: <value>%1</value>%2")
                    .arg(toNumStr(TagSliceThickness, 2), millimeter);
        else if (!dataConverter.hasTagWithData(TagSliceThickness) && dataConverter.hasTagWithData(TagSliceLocation))
            lines << QObject::tr("Slice location: <value>%1</value>%2")
                    .arg(toNumStr(TagSliceLocation, 2), millimeter);
    }

    //region Modality Specials


    if (modality == "CT") return genTextCT();
//	else if (modality == "US") return genTextUS();
    else if (modality == "RT") return genTextRT();
    else if (modality == "CR") return genTextRT();
    else if (modality == "MR") return genTextMR();

    //endregion


}

void Modality::reposition() {

    text->setPos(0, scene()->height() - text->document()->size().height());
}

void Modality::genTextUS() {
    //http://dicom.nema.org/medical/dicom/current/output/chtml/part03/sect_C.8.5.6.html
}

void Modality::genTextCT() {
    //http://dicom.nema.org/medical/dicom/current/output/chtml/part03/sect_C.8.3.html

    const static gdcm::Tag
            TagKVP(0x0018, 0x0060),
            TagExposureTime(0x0018, 0x1150),
            TagExposure(0x0018, 0x1152);

    if (dataConverter.hasTagWithData(TagKVP))
        lines << QObject::tr("KVP: <value>%1</value><unit>kV</unit>").arg(toNumStr(TagKVP));

    if (dataConverter.hasTagWithData(TagExposureTime))
        lines << QObject::tr("Exposure time: <value>%1</value><unit>ms</unit>").arg(toNumStr(TagExposureTime));

    if (dataConverter.hasTagWithData(TagExposure))
        lines << QObject::tr("Exposure: <value>%1</value><unit>mAs</unit>").arg(toNumStr(TagExposure));
}

void Modality::genTextMR() {
    //http://dicom.nema.org/medical/dicom/current/output/chtml/part03/sect_C.8.3.html

    const static gdcm::Tag
            TagRepetitionTime(0x0018, 0x0080),
            TagEchoTime(0x0018, 0x0081),
            TagMagneticFieldStrength(0x0018, 0x0087),
            TagSAR(0x0018, 0x1316);


    if (dataConverter.hasTagWithData(TagRepetitionTime))
        lines << QObject::tr("Repetition time: <value>%1</value><unit>ms</unit>").arg(toNumStr(TagRepetitionTime));

    if (dataConverter.hasTagWithData(TagEchoTime))
        lines << QObject::tr("Echo time: <value>%1</value><unit>ms</unit>").arg(toNumStr(TagEchoTime));

    if (dataConverter.hasTagWithData(TagMagneticFieldStrength))
        lines << QObject::tr("Magnetic field: <value>%1</value><unit>T</unit>").arg(toNumStr(TagMagneticFieldStrength));

    if (dataConverter.hasTagWithData(TagSAR))
        lines << QObject::tr("SAR: <value>%1</value><unit>W/kg</unit>").arg(toStr(TagSAR));
}

void Modality::genTextRT() {
    //http://dicom.nema.org/medical/dicom/current/output/chtml/part03/sect_C.8.8.2.html

    const static gdcm::Tag
            TagKVP(0x0018, 0x0060),
            TagExposureTime(0x0018, 0x1150);

    if (dataConverter.hasTagWithData(TagExposureTime))
        lines << QObject::tr("Exposure time: <value>%1</value><unit>ms</unit>").arg(toNumStr(TagExposureTime));

    if (dataConverter.hasTagWithData(TagKVP))
        lines << QObject::tr("KVP: <value>%1</value><unit>kV</unit>").arg(toNumStr(TagKVP));
}

bool Modality::isAlive() {

    return true;
}


