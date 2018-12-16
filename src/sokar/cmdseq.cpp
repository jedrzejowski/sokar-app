#include "cmdseq.h"

using namespace Sokar;

CommandSequence::CommandSequence(QObject *parent) : QObject(parent) {

}


CommandSequence &CommandSequence::operator<<(const Command &cmd) {
	cmds << cmd;
	return *this;
}

void CommandSequence::step() {
	currentStep++;

	currentStep %= size();

	auto &currentCmd = cmds[currentStep];

	switch (currentCmd.type) {

		case Sleep:
			onSleep(currentCmd.arg);
			break;

		case GoTo:
			onGoTo(currentCmd.arg);
			break;

		case End:
			onEnd(currentCmd.arg);
			break;
	}

}

