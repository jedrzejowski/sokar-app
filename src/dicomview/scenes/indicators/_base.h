#pragma once

#include <QtCore>
#include <QtWidgets>

#include "../../_classdef.h"
#include "../scene.h"

namespace Sokar {

	class SceneIndicator : public QGraphicsItemGroup {
	private:
		qreal offsetRight = 0, offsetTop = 0, offsetLeft = 0, offsetBottom = 0;
		SceneIndicator *offsetRightParent = nullptr, *offsetTopParent = nullptr,
				*offsetLeftParent = nullptr, *offsetBottomParent = nullptr;
	protected:
		QColor defaultColor;
	public:
		SceneIndicator();

		virtual void reposition() = 0;

		//region Getters & Setters

		qreal getOffsetRight() const;
		void setOffsetRight(qreal offsetRight);
		qreal getOffsetTop() const;
		void setOffsetTop(qreal offsetTop);
		qreal getOffsetLeft() const;
		void setOffsetLeft(qreal offsetLeft);
		qreal getOffsetBottom() const;
		void setOffsetBottom(qreal offsetBottom);

		virtual qreal getTopSpace();
		virtual qreal getBottomSpace();
		virtual qreal getRightSpace();
		virtual qreal getLeftSpace();

		SceneIndicator *getOffsetRightParent() const;
		void setOffsetRightParent(SceneIndicator *offsetRightParent);
		SceneIndicator *getOffsetTopParent() const;
		void setOffsetTopParent(SceneIndicator *offsetTopParent);
		SceneIndicator *getOffsetLeftParent() const;
		void setOffsetLeftParent(SceneIndicator *offsetLeftParent);
		SceneIndicator *getOffsetBottomParent() const;
		void setOffsetBottomParent(SceneIndicator *offsetBottomParent);

		//endregion


	};


}