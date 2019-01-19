#include "palette.h"

using namespace Sokar;


QVector<Palette *> &Palette::getAll() {
	static QMutex mutex;
	static QVector<Palette *> all;

	if (!all.isEmpty())
		return all;

	mutex.lock();

	if (!all.isEmpty())
		return all;

	all << getMono1();
	all << getMono2();

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

	return all;
}

Palette *Palette::fromFile(QFile &file) {

	if (!file.open(QFile::ReadOnly))
		return nullptr;

	QXmlStreamReader reader(&file);

	auto palette = new Palette;

	while (true) {
		auto token = reader.readNext();

		if (token == QXmlStreamReader::EndDocument)
			break;

		if (token == QXmlStreamReader::StartElement) {
			if (reader.name() == "palette") {
				auto attrs = reader.attributes();
				palette->name = attrs.value("name").toString();
				palette->display = attrs.value("display").toString();
			}

			if (reader.name() == "entry") {

				auto attrs = reader.attributes();

				palette->pixels << Pixel(
						(quint8) attrs.value("red").toUShort(),
						(quint8) attrs.value("green").toUShort(),
						(quint8) attrs.value("blue").toUShort()
				);
			}
		}
	}


	return palette;

}

Palette *Palette::getMono1() {
	static Palette *palette;

	if (palette != nullptr) return palette;

	palette = new Palette();

	palette->name = "";
	palette->display = "Monochrome1";
	palette->pixels.resize(256);

	for (int i = 0; i <= 255; i++)
		palette->pixels[255 - i] = Pixel(quint8(i));

	return palette;
}

Palette *Palette::getMono2() {
	static Palette *palette;

	if (palette != nullptr) return palette;

	palette = new Palette();

	palette->name = "";
	palette->display = "Monochrome2";
	palette->pixels.resize(256);

	for (int i = 0; i <= 255; i++)
		palette->pixels[i] = Pixel(quint8(i));

	return palette;
}
