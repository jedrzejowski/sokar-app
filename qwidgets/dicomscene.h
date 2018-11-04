#pragma once

#include <QGraphicsView>
#include <gdcmImage.h>

namespace Sokar {
	class DicomScene;
}

class Sokar::DicomScene : public QGraphicsScene {
Q_OBJECT

private:
	gdcm::Image *gdcmImage = nullptr;

	QGraphicsTextItem *text11, *text12, *text13, *text21, *text22, *text23, *text31, *text32, *text33;

	int maxWidth, maxHeight;

public:
	explicit DicomScene(gdcm::Image *gdcmImage);

	~DicomScene() override;

	void resize(int,int);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

	void wheelEvent(QGraphicsSceneWheelEvent *event) override;

private:
	void positTxts();
};
