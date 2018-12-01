#include <QtCore>

#include "pixel.h"

namespace Sokar {
	class Palette : public QObject {
	Q_OBJECT
	private:

		QString name, display;
		QList<Pixel> pixels;

	public:
		static QList<Palette *> &getAll();
		static Palette *fromFile(QFile &file);

		inline int size() {
			return pixels.size();
		}

		inline const Pixel &getPixel(int i) {
			return pixels[i];
		}

		inline const Pixel &getPixel(float i) {
			return getPixel(int(i * size()));
		}

		inline QString getDisplay() { return display; }

		inline QString getName() { return name; }

	};
}