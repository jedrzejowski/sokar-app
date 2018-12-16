#pragma once

#include <QtCore>
#include <QtWidgets>

#include "../_classdef.h"
#include "indicators/_base.h"

namespace Sokar {
	class Scene : public QGraphicsScene {
	Q_OBJECT
	private:
		QVector<SceneIndicator *> indicators;

	public:

		explicit Scene(QObject *parent = nullptr);

		~Scene() override;

		void addIndicator(SceneIndicator *indicator);

		void removeIndicator(SceneIndicator *indicator);

		virtual void reposItems();

	protected:

		SceneIndicator *findIndicatorByChild(QGraphicsItem *item);

	};
}