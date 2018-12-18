#pragma once

#include <QtCore>

#include "../dicomscene.h"

namespace Sokar {

	class Step : public QObject {
	Q_OBJECT
	public:
		DicomScene *scene;
		quint64 time;

		Step(DicomScene *scene, quint64 time) : scene(scene), time(time) {}

		inline bool operator==(const Step &step) {
			return this == &step;
		}
	};

	class SceneSequence : public QObject {
	Q_OBJECT
	private:
		QVector<const Step *> steps;
		int index = -1;
		int direction = 1;

		bool sweeping = false;

	public:
		explicit SceneSequence(QObject *parent = nullptr);

		inline int size() const { return steps.size(); }

		inline int currentIndex() { return index; }

		inline bool isSweeping() const { return sweeping; }

		inline int getDirection() const { return direction; }

		int indexOf(const Step *step);

		void setSweeping(bool sweeping);

		void reset();

		SceneSequence &operator<<(Step *step);

		SceneSequence &operator<<(SceneSequence *sceneSequence);

		inline const Step *operator[](int index) { return steps[index]; }

	public slots:
		const Step *step();
		const Step *stepForward();
		const Step *stepBackward();

	signals:
		void steped(const Step *step);
	};
}
