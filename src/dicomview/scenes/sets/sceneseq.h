#pragma once

#include <QtCore>

#include "../dicomscene.h"

namespace Sokar {

	struct Step {
		DicomScene *scene;
		quint64 time;
	};

	class SceneSequence : public QObject {
	Q_OBJECT
	private:
		QVector<Step> steps;
		int index = -1;
		int direction = 1;

		bool sweeping = false;

	public:
		explicit SceneSequence(QObject *parent = nullptr);

		inline int size() const { return steps.size(); }

		inline bool isSweeping() const { return sweeping; }

		void setSweeping(bool sweeping);

		void reset();

		SceneSequence &operator<<(const Step &cmd);

		SceneSequence &operator<<(const SceneSequence &sceneSequence);

	public slots:
		void step();

	signals:
		void steped(const Step &step);
	};

	inline SceneSequence *operator<<(SceneSequence *cs, const Step &cmd) {
		(*cs) << cmd;
		return cs;
	}
}
