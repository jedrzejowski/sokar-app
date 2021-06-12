//
// Created by adam on 12.06.2021.
//

#pragma once

#include <QString>
#include <QFile>
#include <QByteArray>

namespace SokarLib {
	class Saveable {
	public:
		QString saveWithGui() const;
		void save2file(QFile &file) const;
		virtual QByteArray dump2byteArray() const = 0;
	};
}



