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

class Command {
public:
    Command(const std::string &base);
    virtual ~Command();

    virtual void execute(const CommandContext &c) = 0;
    bool globalEvent() const { return mGlobal; }

    const std::string &base() const { return mBase; }
    const std::string &description() const { return mDescription; }

    int minParameters() const { return mMinParameters; }
    int maxParameters() const { return mMaxParameters; }
private:
    std::string mBase;
    std::string mDescription;
    int mMinParameters, mMaxParameters;
    bool mGlobal;
};

#endif // COMMAND_H
