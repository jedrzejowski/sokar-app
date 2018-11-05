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
	char *originBuffer;
	uchar *targetBuffer;
	uint dimX, dimY;

	int winCenter, winLength;

public:
	explicit Monochrome2DicomScene(gdcm::Image *gdcmImage);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

protected:
	const QPixmap genQPixmap() override;

};

