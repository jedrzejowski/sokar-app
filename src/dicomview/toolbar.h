#pragma once

#include <QtCore>
#include <QtWidgets>

#include "_classdef.h"

namespace Sokar {
	class DicomToolBar : public QToolBar {
	Q_OBJECT

	public:
		enum State {
			None,
			Windowing,
			Pan,
			Zoom,
			Rotate,
			Custom
		};

		enum Action {
			ClearPan,
			Fit2Screen,
			OriginalResolution,
			RotateRight90,
			RotateLeft90,
			FlipHorizontal,
			FlipVertical,
			ClearRotate,
			OpenDataSet
		};

	private:
		enum State state = None;

		QActionGroup *toggleActionGrp;
		QAction *actionWindowing, *actionPan, *actionZoom, *actionRotate, *actionTags;

	public:
		explicit DicomToolBar(QWidget *parent);

		//region Getters

		inline const State &getState() const { return state; }

		inline QAction *getActionWindowing() const { return actionWindowing; }

		inline QAction *getActionPan() const { return actionPan; }

		inline QAction *getActionZoom() const { return actionZoom; }

		inline QAction *getActionRotate() const { return actionRotate; }

		inline QAction *getActionTags() const { return actionTags; }

		//endregion

	private:
		void initActions();

	signals:
		void stateToggleSignal(State state);
		void actionTriggerSignal(Action action);
	};
}


