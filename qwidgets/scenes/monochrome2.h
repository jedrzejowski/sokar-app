#pragma once

#include <QtCore>
#include <qwidgets/dicomscene.h>
#include "sokar/imgwindow.h"

namespace Sokar {
	class Monochrome2DicomScene;
}

class Sokar::Monochrome2DicomScene : public Sokar::DicomScene {
Q_OBJECT

protected:
	ImgWindow imgWindow;
	std::vector<char> originVectorBuffer;
	uchar *targetBuffer;
	uint dimX, dimY;
	
	ushort bitsStored;

public:
	explicit Monochrome2DicomScene(const gdcm::File &gdcmFile, const gdcm::Image &gdcmImage);

	~Monochrome2DicomScene() override;

protected:
	
	bool genQPixmap() override;

	void readAttributes();
};

