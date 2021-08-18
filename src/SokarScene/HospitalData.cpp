#include "HospitalData.hpp"

using namespace SokarScene;

HospitalData::HospitalData(SokarDicom::DataConverter &dataConverter) : Indicator(dataConverter) {

    text = newText();
    addToGroup(text);

    initData();
}

void HospitalData::reposition() {

    text->setPos(scene()->width() - text->document()->size().width(), 0);
}

void HospitalData::initData() {

    const static gdcm::Tag
            TagManufacturer(0x0008, 0x0070),
            TagInstitutionalDepartmentName(0x0008, 0x1040),
            TagOperatorsName(0x0008, 0x1070),
            TagManufacturerModelName(0x0008, 0x1090),
            TagReferringPhysicianName(0x0008, 0x0090);

    QString htmlTags = " style='white-space: nowrap;' align='right' ";

    QStringList lines;

    if (dataConverter.hasTagWithData(TagInstitutionalDepartmentName)) {
        lines << dataConverter.toString(TagInstitutionalDepartmentName).trimmed();
    }

    if (dataConverter.hasTagWithData(TagManufacturer)) {
        QString manufacture = dataConverter.toString(TagManufacturer).trimmed();
        QString model = dataConverter.toString(TagManufacturerModelName).trimmed();

        lines << QObject::tr("%1 %2").arg(manufacture, model).trimmed();
    }

    if (dataConverter.hasTagWithData(TagReferringPhysicianName)) {
        lines << dataConverter.toString(TagReferringPhysicianName).trimmed();
    }

    if (dataConverter.hasTagWithData(TagOperatorsName)) {
        lines << dataConverter.toString(TagOperatorsName).trimmed();
    }

    text->setHtml(wrapAsHtml(lines, true));
    text->adjustSize();
}

bool HospitalData::isAlive() {

    return true;
}
