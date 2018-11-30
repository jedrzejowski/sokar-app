#pragma once

#include <QtCore>
#include <QtWidgets>

#include "_classdef.h"

namespace Sokar {
	class DicomToolBar : public QToolBar {
	Q_OBJECT

	public:
		enum State {
			Windowing,
			Pan,
			Zoom,
			Rotate,
			Custom
		};

		enum Action {
			OpenDataSet
		};

	private:
		enum State state;

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


