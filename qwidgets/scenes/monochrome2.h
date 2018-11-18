#pragma once

#include <QtCore>

#include "qwidgets/dicomscene.h"

#include "sokar/windowing/imgwindowint.h"

namespace Sokar {
	class Monochrome2DicomScene;
}

class Sokar::Monochrome2DicomScene : public Sokar::DicomScene {
Q_OBJECT

protected:
	std::vector<char> originVectorBuffer;
	Pixel *targetBuffer;
	uint dimX, dimY;

	ImgWindowINT* imgWindowInt;

public:
	explicit Monochrome2DicomScene(const gdcm::ImageReader &imageReader, SceneParams *sceneParams);

	~Monochrome2DicomScene() override;

protected:
	
	bool genQPixmap() override;

	void readAttributes();
};

