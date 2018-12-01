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

		QActionGroup* toggleActionGrp;

	public:
		explicit DicomToolBar(QWidget *parent);

		//region Getters

		inline const State &getState() const {
			return state;
		}

		//endregion

	private:
		void initActions();

	signals:
		void stateToggleSignal(State state);
		void actionTriggerSignal(Action action);
	};
}


