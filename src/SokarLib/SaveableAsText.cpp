//
// Created by adam on 12.06.2021.
//

#include "SaveableAsText.hpp"

using namespace SokarLib;

void SaveableAsText::save2file(QFile &file) const {
	QTextStream out(&file);
	dump2stream(out);
}
