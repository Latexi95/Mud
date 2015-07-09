#ifndef MESSAGEBUILDER_H
#define MESSAGEBUILDER_H
#include <string>
#include <vector>
#include "name.h"
class Item;

class MessageBuilder {
public:
    friend MessageBuilder &operator<< (MessageBuilder &mb, int i);
    friend MessageBuilder &operator<< (MessageBuilder &mb, const std::string &str);
    friend MessageBuilder &operator<< (MessageBuilder &mb, const Name &name);
    friend MessageBuilder &operator<< (MessageBuilder &mb, const std::unique_ptr<Item> &item);
    MessageBuilder();
    ~MessageBuilder();

    operator std::string();

private:
    std::vector<std::string> mParts;
    int mNumber;
};


MessageBuilder &operator<< (MessageBuilder &mb, int i);
MessageBuilder &operator<< (MessageBuilder &mb, const std::string &str);
MessageBuilder &operator<< (MessageBuilder &mb, const Name &name);
MessageBuilder &operator<< (MessageBuilder &mb, const std::unique_ptr<Item> &item);

#endif // MESSAGEBUILDER_H
