#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H
#include "command.h"
#include <memory>
#include <vector>
#include "gameeventloop.h"

class CommandParser {
	public:
		CommandParser(const std::shared_ptr<GameEventLoop> &global, const std::shared_ptr<GameEventLoop> &local);
		~CommandParser();
		Command *parse(const std::string &cmd, std::vector<std::string> &params);

		void addCommand(std::unique_ptr<Command> &&command);

		const std::string &errorMessage() const { return mErrorMessage; }

	private:
		typedef std::vector<std::unique_ptr<Command> > CmdVector;
		CmdVector::iterator closestCommand(const std::string &base);

		CmdVector mCommands;
		std::shared_ptr<GameEventLoop> mGlobalEventLoop;
		std::shared_ptr<GameEventLoop> mLocalEventLoop;
		std::string mErrorMessage;
};

#endif // COMMANDPARSER_H
