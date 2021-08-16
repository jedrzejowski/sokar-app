#pragma once

#include <gdcmImageReader.h>

#include <QtCore>
#include "SokarScene/SceneParams.hpp"

#include "../dicomscene.h"

namespace Sokar {
	class ExceptionScene : public Sokar::DicomScene {
	Q_OBJECT

	private:
		QGraphicsTextItem *msgText;
		Sokar::Exception *exception;

	public:
		explicit ExceptionScene(SokarScene::SceneParams &sceneParams, Sokar::Exception &exception);

		~ExceptionScene() override;

	protected:
		bool generatePixmap() override;

	public:
		void toolBarAdjust() override;

	public:
		void reposItems() override;
	};
}

