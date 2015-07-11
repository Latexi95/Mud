#include "messagebuilder.h"
#include "item.h"
#include <boost/lexical_cast.hpp>
#include "character.h"

MessageBuilder::MessageBuilder(const std::string &str) :
    mParts(1, Part(Default, str)),
    mNumber(-1)
{
}

MessageBuilder::MessageBuilder(std::string &&str) :
    mParts(1, Part(Default, std::move(str))),
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
    mParts.emplace_back(Default, str);
    mNumber = -1;
    return *this;
}

MessageBuilder &MessageBuilder::operator=(std::string &&str)
{
    mParts.clear();
    mParts.emplace_back(Default, std::move(str));
    mNumber = -1;
    return *this;
}

std::string MessageBuilder::generateTelnetString() const
{
    if (mParts.empty()) return std::string();

    std::string result;
    int resultSize = 2;
    int style = Default;
    for (const Part &part : mParts) {
        resultSize += part.mText.size() + 1;
        if (style != part.mStyle) {
            if (style != Default) resultSize += 2 + 1 + 1; //reset to default
            if (part.mStyle != Default) {
                resultSize += 1 + (int)bolded(style) * 2 + (int)underlined(style) * 2 + (int)foregroundColorSet(style) * 3 + (int)backgroundColorSet(style) * 3;
            }
            style = part.mStyle;
        }
    }


    std::string endPart;
    if (mNumber >= 0) {
        if (style != Default) resultSize += 2 + 1 + 1; //reset to default
        endPart = boost::lexical_cast<std::string>(mNumber);
        resultSize += endPart.size();
    }

    style = Default;
    result.reserve(resultSize);
    std::vector<Part>::const_iterator it = mParts.begin();
    for (; it != --mParts.end(); ++it) {
        if (it->mStyle != style) {
            if (style != Default) {
                result += "\x1b[0m";
            }
            if (it->mStyle != Default) {
                bool others = false;
                result += "\x1b[";
                if (bolded(it->mStyle)) {
                    result += '1';
                    others = true;
                }
                if (underlined(it->mStyle)) {
                    if (others) result += ';';
                    result += '4';
                    others = true;
                }
                if (foregroundColorSet(style)) {
                    if (others) result += ';';
                    result += '3';
                    result += foregroundTelnetColorCode(style);
                    others = true;
                }
                if (backgroundColorSet(style)) {
                    if (others) result += ';';
                    result += '4';
                    result += backgroundTelnetColorCode(style);
                    others = true;
                }
            }
            style = it->mStyle;
        }
        result += it->mText;
        if (it->mText.back() != ' ') result += ' ';
    }

    if (it->mStyle != style) {
        if (style != Default) {
            result += "\x1b[0m";
        }
        if (it->mStyle != Default) {
            bool others = false;
            result += "\x1b[";
            if (bolded(it->mStyle)) {
                result += '1';
                others = true;
            }
            if (underlined(it->mStyle)) {
                if (others) result += ';';
                result += '4';
                others = true;
            }
            if (foregroundColorSet(style)) {
                if (others) result += ';';
                result += '3';
                result += foregroundTelnetColorCode(style);
                others = true;
            }
            if (backgroundColorSet(style)) {
                if (others) result += ';';
                result += '4';
                result += backgroundTelnetColorCode(style);
                others = true;
            }
        }
        style = it->mStyle;
    }


    result += it->mText;

    if (mNumber >= 0) {
        if (style != Default) {
            result += "\x1b[0m";
        }
        if (it->mText.back() != ' ') result += ' ';
        result += endPart;
    }
    result += "\r\n";

    return result;
}

void MessageBuilder::append(const std::string &str)
{
    clearNumberStash();
    mParts.emplace_back(Default, str);
}

void MessageBuilder::append(std::string &&str)
{
    clearNumberStash();
    mParts.emplace_back(Default, std::move(str));
}

void MessageBuilder::append(int num)
{
    clearNumberStash();
    mNumber = num;
}

void MessageBuilder::append(const Name &name)
{
    if (mNumber >= 0) {
        mParts.emplace_back(Default, name.num(mNumber, true));
        mNumber = -1;
    }
    else {
        mParts.emplace_back(Default, name.num(1, true));
    }
}

void MessageBuilder::append(const std::unique_ptr<Item> &item)
{
    if (mNumber >= 0) {
        mParts.emplace_back(Bold | FGBlue, item->name().num(mNumber, true));
        mNumber = -1;
    }
    else {
        mParts.emplace_back(Bold | FGBlue, item->name().num(1, true));
    }
}

void MessageBuilder::append(const std::shared_ptr<Character> &character)
{
    clearNumberStash();
    mParts.emplace_back(Bold | FGYellow, character->name());
}

bool MessageBuilder::underlined(int style)
{
    return (style & Underline) == Underline;
}

bool MessageBuilder::bolded(int style)
{
    return (style & Bold) == Bold;
}

bool MessageBuilder::foregroundColorSet(int style)
{
    return (style & 0x3C) != 0;
}

bool MessageBuilder::backgroundColorSet(int style)
{
    return (style & 0x3C0) != 0;
}


MessageBuilder &MessageBuilder::operator<<(int i)
{
    append(i);
    return *this;
}

MessageBuilder &MessageBuilder::operator<<(const std::string &str)
{
    append(str);
    return *this;
}

MessageBuilder &MessageBuilder::operator<<(const Name &name)
{
    append(name);
    return *this;
}




MessageBuilder &MessageBuilder::operator<<(const std::unique_ptr<Item> &item)
{
    append(item);
    return *this;
}


MessageBuilder &MessageBuilder::operator<<(std::string &&str)
{
    append(std::move(str));
    return *this;
}


MessageBuilder &MessageBuilder::operator<<(const std::shared_ptr<Character> &character)
{
    append(character);
    return *this;
}



char MessageBuilder::foregroundTelnetColorCode(int style)
{
    return '0' + ((style & 0x3C) >> 2) - 1;
}

char MessageBuilder::backgroundTelnetColorCode(int style)
{
    return '0' + ((style & 0x3C0) >> 6) - 1;
}

void MessageBuilder::clearNumberStash()
{
    if (mNumber >= 0) {
        mParts.emplace_back(Default, boost::lexical_cast<std::string>(mNumber));
        mNumber = -1;
    }
}



