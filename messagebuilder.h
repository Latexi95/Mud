#ifndef MESSAGEBUILDER_H
#define MESSAGEBUILDER_H
#include <string>
#include <vector>
#include "name.h"
class Item;

class MessageBuilder {
public:
    MessageBuilder(const std::string &str);
    MessageBuilder(std::string &&str);
    MessageBuilder(const MessageBuilder &mb);
    MessageBuilder(MessageBuilder &&mb);
    ~MessageBuilder();

    MessageBuilder &operator =(const MessageBuilder &mb);
    MessageBuilder &operator =(MessageBuilder &&mb);

    MessageBuilder &operator =(const std::string &str);
    MessageBuilder &operator =(std::string &&str);

    operator std::string();

    void append(const std::string &str);
    void append(std::string &&str);
    void append(int num);
    void append(const Name &name);
    void append(const std::unique_ptr<Item> &item);
private:
    void clearNumberStash();

    std::vector<std::string> mParts;
    int mNumber;
};


MessageBuilder &operator<< (MessageBuilder &mb, int i);
MessageBuilder &operator<< (MessageBuilder &mb, const std::string &str);
MessageBuilder &operator<< (MessageBuilder &mb, std::string &&str);
MessageBuilder &operator<< (MessageBuilder &mb, const Name &name);
MessageBuilder &operator<< (MessageBuilder &mb, const std::unique_ptr<Item> &item);

#endif // MESSAGEBUILDER_H
