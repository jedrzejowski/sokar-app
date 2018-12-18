#include "sceneseq.h"

using namespace Sokar;

SceneSequence::SceneSequence(QObject *parent) : QObject(parent) {

}

const Step *SceneSequence::step() {
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
		direction = 1;
		index %= steps.size();
	}

	qDebug() << index;
	emit steped(steps[index]);
	return steps[index];
}

void SceneSequence::setSweeping(bool sweeping) {
	SceneSequence::sweeping = sweeping;

	if (sweeping) {
		direction = 1;
	}
}

SceneSequence &SceneSequence::operator<<(Step *step) {
	steps << step;
	step->setParent(this);
	return *this;
}

SceneSequence &SceneSequence::operator<<(SceneSequence *sceneSequence) {

	for (auto &step : sceneSequence->steps) {
		steps << step;
		//TODO tu będzie wywałka
	}

	return *this;
}

void SceneSequence::reset() {
	index = -1;
	direction = 1;
}

int SceneSequence::indexOf(const Step *step) {
	return steps.indexOf(step);
}
