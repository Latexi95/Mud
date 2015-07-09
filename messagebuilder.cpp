#include "messagebuilder.h"
#include "item.h"
#include <boost/lexical_cast.hpp>

MessageBuilder::MessageBuilder(const std::string &str) :
    mParts(1, str),
    mNumber(-1)
{
}

MessageBuilder::MessageBuilder(std::string &&str) :
    mParts(1, std::move(str)),
    mNumber(-1)
{
}

MessageBuilder::MessageBuilder(const MessageBuilder &mb) :
    mParts(mb.mParts),
    mNumber(mb.mNumber)
{
}

MessageBuilder::MessageBuilder(MessageBuilder &&mb) :
    mParts(std::move(mb.mParts)),
    mNumber(mb.mNumber)
{

}

MessageBuilder::~MessageBuilder()
{

}

MessageBuilder &MessageBuilder::operator=(const MessageBuilder &mb)
{
    mParts = mb.mParts;
    mNumber = mb.mNumber;
    return *this;
}

MessageBuilder &MessageBuilder::operator=(MessageBuilder &&mb)
{
    mParts = std::move(mb.mParts);
    mNumber = mb.mNumber;
    return *this;
}

MessageBuilder &MessageBuilder::operator=(const std::string &str)
{
    mParts.clear();
    mParts.emplace_back(str);
    mNumber = -1;
    return *this;
}

MessageBuilder &MessageBuilder::operator=(std::string &&str)
{
    mParts.clear();
    mParts.emplace_back(std::move(str));
    mNumber = -1;
    return *this;
}

void MessageBuilder::append(const std::string &str)
{
    clearNumberStash();
    mParts.emplace_back(str);
}

void MessageBuilder::append(std::string &&str)
{
    clearNumberStash();
    mParts.emplace_back(std::move(str));
}

void MessageBuilder::append(int num)
{
    clearNumberStash();
    mNumber = num;
}

void MessageBuilder::append(const Name &name)
{
    if (mNumber >= 0) {
        mParts.emplace_back(name.num(mNumber, true));
        mNumber = -1;
    }
    else {
        mParts.emplace_back(name.num(1, true));
    }
}

void MessageBuilder::append(const std::unique_ptr<Item> &item)
{
    if (mNumber >= 0) {
        mParts.emplace_back(item->name().num(mNumber, true));
        mNumber = -1;
    }
    else {
        mParts.emplace_back(item->name().num(1, true));
    }
}

void MessageBuilder::clearNumberStash()
{
    if (mNumber >= 0) {
        mParts.emplace_back(boost::lexical_cast<std::string>(mNumber));
        mNumber = -1;
    }
}

MessageBuilder::operator std::string()
{
    if (mParts.empty()) return std::string();

    std::string result;
    int resultSize = 0;
    for (const std::string &part : mParts) {
        resultSize += part.size() + 1;
    }


    std::string endPart;
    if (mNumber >= 0) {
        endPart = boost::lexical_cast<std::string>(mNumber);
        resultSize += endPart.size();
    }

    result.reserve(resultSize);
    std::vector<std::string>::const_iterator it = mParts.begin();
    for (; it != --mParts.end(); ++it) {
        result += *it;
        if (it->back() != ' ') result += ' ';
    }
    result += *it;

    if (mNumber >= 0) {
        if (it->back() != ' ') result += ' ';
        result += endPart;
    }

    return result;
}

MessageBuilder &operator<<(MessageBuilder &mb, int i)
{
    mb.append(i);
    return mb;
}

MessageBuilder &operator<<(MessageBuilder &mb, const std::string &str)
{
    mb.append(str);
    return mb;
}

MessageBuilder &operator<<(MessageBuilder &mb, const Name &name)
{
    mb.append(name);
    return mb;
}




MessageBuilder &operator<<(MessageBuilder &mb, const std::unique_ptr<Item> &item)
{
    mb.append(item);
    return mb;
}


MessageBuilder &operator<<(MessageBuilder &mb, std::string &&str)
{
    mb.append(std::move(str));
    return mb;
}
