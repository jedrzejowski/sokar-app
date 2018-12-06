#pragma once

#include <QtCore>
#include <QtWidgets>

#include "../_classdef.h"
#include "indicators/_base.h"

namespace Sokar {
	class Scene : public QGraphicsScene {
	Q_OBJECT
	protected:
		std::vector<SceneIndicator *> indicators;

		QHBoxLayout layoutH1, layoutH2;
		QVBoxLayout layoutV1, layoutV2;

	public:

		explicit Scene(QObject *parent = nullptr);

		~Scene() override;

		const std::vector<SceneIndicator *> &allIndicators() const {
			return indicators;
		}

		void addIndicator(SceneIndicator *indicator);

		void removeIndicator(SceneIndicator *indicator);

		virtual void reposItems();

	protected:

		SceneIndicator *findIndicatorByChild(QGraphicsItem *item);

	};
}