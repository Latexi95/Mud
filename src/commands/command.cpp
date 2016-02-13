#include "command.h"

Command::Command() :
    mMinParameters(0),
    mMaxParameters(0)
{
}

Command::Command(const std::string &base, const std::string &usage) :
    mBase(base),
    mUsage(usage),
    mMinParameters(1),
    mMaxParameters((unsigned)-1)
{

}

Command::Command(const std::string &base, const std::string &usage, unsigned minParams, unsigned maxParams) :
    mBase(base),
    mUsage(usage),
    mMinParameters(minParams),
    mMaxParameters(maxParams)
{

}

Command::~Command()
{

}

bool Command::limitlessLastParameter() const
{
    return mMaxParameters == (unsigned)-1;
}
