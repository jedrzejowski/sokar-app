#include "palette.h"

using namespace Sokar;


QList<Palette *> &Palette::getAll() {
	static QMutex mutex;
	static QList<Palette *> all;

	if (!all.isEmpty())
		return all;

	mutex.lock();

	if (!all.isEmpty())
		return all;

	QDir directory("res/values/colorpalettes");

	auto files = directory.entryList(QStringList() << "*.xml", QDir::Files);

	for (auto &fileName : files) {
		try {
			auto file = QFile(directory.absoluteFilePath(fileName));
			auto pallete = fromFile(file);

			if (pallete != nullptr)
				all << pallete;

		} catch (...) {}
	}

	mutex.unlock();

	// xD jak zakomentuje tego returna to się skompluje xD
	return all;
}

Palette *Palette::fromFile(QFile &file) {

	if (!file.open(QFile::ReadOnly))
		return nullptr;

	QXmlStreamReader reader(&file);

	auto palette = new Palette;

	if (reader.readNextStartElement()) {
		if (reader.name() == "palette") {

			{
				auto attrs = reader.attributes();
				palette->name = attrs.value("name").toString();
				palette->display = attrs.value("display").toString();
			}

			while (reader.readNextStartElement()) {
				if (reader.name() == "entry") {

					auto attrs = reader.attributes();

					palette->pixels << Pixel(
							(quint8) attrs.value("red").toUShort(),
							(quint8) attrs.value("green").toUShort(),
							(quint8) attrs.value("blue").toUShort()
					);

				} else reader.skipCurrentElement();
			}

		} else {
			delete palette;
			return nullptr;
		}
	}

	return palette;

}
