//
// Created by adam on 12.06.2021.
//

#include <QTextStream>
#include "Saveable.h"

using namespace SokarLib;

void Saveable::save2file(QFile &file) const {
	QTextStream out(&file);

	out << dump2byteArray();
}

QString Saveable::saveWithGui() const {

	return QString();
}
