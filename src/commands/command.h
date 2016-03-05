#ifndef COMMAND_H
#define COMMAND_H
#include <string>
#include <memory>
#include <vector>

class Character;
struct CommandContext {
    std::shared_ptr<Character> mCaller;
    std::vector<std::string> mParameters;
};
class UI;

class Command {
public:
    Command();
    Command(const std::string &base, const std::string &usage);
    Command(const std::string &base, const std::string &usage, unsigned minParams, unsigned maxParams = (unsigned)-1);
    virtual ~Command();

    virtual void execute(const CommandContext &c, UI &messageContext) const = 0;
    virtual bool globalEvent() const { return false; }

    const std::string &base() const { return mBase; }
    const std::string &usage() const { return mUsage; }

    unsigned minParameters() const { return mMinParameters; }
    unsigned maxParameters() const { return mMaxParameters; }

    bool limitlessLastParameter() const;
protected:
    std::string mBase;
    std::string mUsage;
    unsigned mMinParameters, mMaxParameters;
};

#endif // COMMAND_H
