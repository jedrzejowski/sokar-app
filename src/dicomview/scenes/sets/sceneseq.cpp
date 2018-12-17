#include "sceneseq.h"

using namespace Sokar;

SceneSequence::SceneSequence(QObject *parent) : QObject(parent) {

}

void SceneSequence::step() {
	index += direction;

	if (sweeping) {

		if (index <= 0) {
			direction = 1;
			index = 0;
		}

		if (index >= steps.size() - 1) {
			direction = -1;
			index = steps.size() - 1;
		}
	} else {
		index %= steps.size();
	}

	auto &step = steps[index];

	emit steped(step);
}

void SceneSequence::setSweeping(bool sweeping) {
	SceneSequence::sweeping = sweeping;
}

SceneSequence &SceneSequence::operator<<(const Step &step) {
	steps << step;
	return *this;
}

SceneSequence &SceneSequence::operator<<(const SceneSequence &sceneSequence) {

	for (auto &step : sceneSequence.steps)
		this << step;

	return *this;
}
