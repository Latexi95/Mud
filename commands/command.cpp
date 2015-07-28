#include "command.h"


Command::Command(const std::string &base, const std::string &usage) :
    mBase(base),
    mUsage(usage),
    mMinParameters(1),
    mMaxParameters(1),
    mSingleParameter(true)
{

}

Command::Command(const std::string &base, const std::string &usage, unsigned minParams, unsigned maxParams) :
    mBase(base),
    mUsage(usage),
    mMinParameters(minParams),
    mMaxParameters(maxParams),
    mSingleParameter(false)
{

}

Command::~Command()
{

}
