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

struct CommandResult {
    bool mSuccess;
    std::string mErrorMessage;
};

class Command {
public:
    Command();
    Command(const std::string &base, const std::string &usage);
    Command(const std::string &base, const std::string &usage, unsigned minParams, unsigned maxParams);
    virtual ~Command();

    virtual CommandResult execute(const CommandContext &c) = 0;
    virtual bool globalEvent() const = 0;

    const std::string &base() const { return mBase; }
    const std::string &usage() const { return mUsage; }

    bool singleParameter() const { return mSingleParameter; }
    unsigned minParameters() const { return mMinParameters; }
    unsigned maxParameters() const { return mMaxParameters; }
protected:
    std::string mBase;
    std::string mUsage;
    unsigned mMinParameters, mMaxParameters;
    bool mSingleParameter;
};

#endif // COMMAND_H
