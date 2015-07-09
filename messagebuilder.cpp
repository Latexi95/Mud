#include "messagebuilder.h"
#include "item.h"
#include <boost/lexical_cast.hpp>

MessageBuilder::MessageBuilder() :
    mNumber(-1)
{
}

MessageBuilder::operator std::string() {
    if (mNumber >= 0) {
        maybeAddSpace();
        mResult += boost::lexical_cast<std::string>(mNumber);
    }
    return mResult;
}

MessageBuilder &MessageBuilder::maybeAddSpace()
{
    if (!mResult.empty() && mResult.back() != ' ') {
        mResult += ' ';
    }
    return *this;
}


MessageBuilder &operator<<(MessageBuilder &mb, int i)
{
    if (mb.mNumber >= 0) {
        mb.maybeAddSpace();
        mb.mResult += boost::lexical_cast<std::string>(mb.mNumber);
    }
    mb.mNumber = i;
    return mb;
}

MessageBuilder &operator<<(MessageBuilder &mb, const std::string &str)
{
    mb.maybeAddSpace();
    if (mb.mNumber >= 0) {
        mb.mResult += boost::lexical_cast<std::string>(mb.mNumber);
        mb.mResult += ' ';
        mb.mNumber = -1;
    }
    mb.mResult += str;
    return mb;
}

MessageBuilder &operator<<(MessageBuilder &mb, const Name &name)
{
    mb.maybeAddSpace();
    if (mb.mNumber >= 0) {
        mb.mResult += name.name(mb.mNumber, true);
        mb.mNumber = -1;
    }
    else {
        mb.mResult += name.name(1, true);
    }
    return mb;
}




MessageBuilder &operator<<(MessageBuilder &mb, const std::unique_ptr<Item> &item)
{
    mb.maybeAddSpace();
    if (mb.mNumber >= 0) {
        mb.mResult += item->name().name(mb.mNumber, true);
        mb.mNumber = -1;
    }
    else {
        mb.mResult += item->name().name(1, true);
    }
    return mb;
}
