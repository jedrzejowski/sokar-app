#pragma once

#include <QtCore>
#include <qwidgets/dicomscene.h>

namespace Sokar {
	class Monochrome2DicomScene;
}

class Sokar::Monochrome2DicomScene : public Sokar::DicomScene {
Q_OBJECT

protected:
	std::vector<char> originVectorBuffer;
	uchar *targetBuffer;
	uint dimX, dimY;

public:
	explicit Monochrome2DicomScene(gdcm::File &gdcmFile, gdcm::Image &gdcmImage);

	~Monochrome2DicomScene() override;

protected:
	
	bool genQPixmap() override;

};

