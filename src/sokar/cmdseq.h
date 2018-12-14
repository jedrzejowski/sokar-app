#pragma once

#include <QtCore>

namespace Sokar {

	enum CmdType {
		Sleep,
		GoTo,
		End,
	};

	struct Command {

		CmdType type;
		int arg1;
	};

	typedef QVector<Command> CommandSequence;
}
