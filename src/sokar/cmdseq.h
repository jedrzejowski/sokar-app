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
		quint64 arg;
	};

	class CommandSequence : public QObject {
	Q_OBJECT
	private:
		QVector<Command> cmds;
		int currentStep = -1;

	public:
		explicit CommandSequence(QObject *parent = nullptr);

		CommandSequence &operator<<(const Command &cmd);

		inline void clear() { cmds.clear(); }

		inline int size() const { return cmds.size(); }

		inline int getCurrentStep() const { return currentStep; }

		inline const Command &getCurrentCommand() const { return cmds[currentStep]; }

	public slots:
		void step();

	signals:
		void onSleep(quint64 howLong);
		void onGoTo(quint64 to);
		void onEnd(quint64 arg);
	};

	inline CommandSequence *operator<<(CommandSequence *cs, const Command &cmd) {
		(*cs) << cmd;
		return cs;
	}
}
