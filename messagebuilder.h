#ifndef MESSAGEBUILDER_H
#define MESSAGEBUILDER_H
#include <string>
#include <vector>
#include "name.h"
class Item;
class Character;

struct SetStyle {
    SetStyle(int style) : mStyle(style) {}
    int mStyle;
};

class Room;
class MessageBuilder {
public:
    static SetStyle reset;

    enum Style {
        Default = 0,
        Bold = 1,
        Underline = 2,

        //Foreground color
        FGBlack = 1 << 2,
        FGRed = 2 << 2,
        FGGreen = 3 << 2,
        FGYellow = 4 << 2,
        FGBlue = 5 << 2,
        FGMagenta = 6 << 2,
        FGCyan = 7 << 2,
        FGWhite = 8 << 2,

        BGBlack = 1 << 6,
        BGRed = 2 << 6,
        BGGreen = 3 << 6,
        BGYellow = 4 << 6,
        BGBlue = 5 << 6,
        BGMagenta = 6 << 6,
        BGCyan = 7 << 6,
        BGWhite = 8 << 6,


        NoSpace = 1 << 10

    };

    MessageBuilder();
    MessageBuilder(const std::string &str);
    MessageBuilder(std::string &&str);
    MessageBuilder(const MessageBuilder &mb);
    MessageBuilder(MessageBuilder &&mb);
    ~MessageBuilder();

    MessageBuilder &operator =(const MessageBuilder &mb);
    MessageBuilder &operator =(MessageBuilder &&mb);

    MessageBuilder &operator =(const std::string &str);
    MessageBuilder &operator =(std::string &&str);

    std::string generateTelnetString() const;

    void append(const std::string &str);
    void append(std::string &&str);
    void append(int num);
    void append(const Name &name);
    void append(const std::unique_ptr<Item> &item);
    void append(const std::shared_ptr<Character> &character);
    void append(const Room *r);

    static bool underlined(unsigned style);
    static bool bolded(unsigned style);
    static bool foregroundColorSet(unsigned style);
    static bool backgroundColorSet(unsigned style);
    static bool styleNoSpace(unsigned style);

    void setStyle(int style);
    int style() const;

    MessageBuilder &operator<< (int i);
    MessageBuilder &operator<< (const std::string &str);
    MessageBuilder &operator<< (std::string &&str);
    MessageBuilder &operator<< (const Name &name);
    MessageBuilder &operator<< (const std::unique_ptr<Item> &item);
    MessageBuilder &operator<< (const std::shared_ptr<Character> &character);
    MessageBuilder &operator<< (const Room *r);

    MessageBuilder &operator<< (const SetStyle &setStyle);
    MessageBuilder &operator<< (Style xorStyle);

    template <size_t S>
    MessageBuilder &operator<< (char text[S]) {
        append(std::string(text, S));
        return *this;
    }
    template <typename Container, typename Func>
    void appendJoin(Container &&c, Func &&f);
private:
    static char foregroundTelnetColorCode(int style);
    static char backgroundTelnetColorCode(int style);

    void clearNumberStash();
    struct Part {
        Part(unsigned style, const std::string &txt) : mText(txt), mStyle(style) {}
        Part(unsigned style, std::string &&txt) : mText(std::move(txt)), mStyle(style) {}
        Part(const Part &p) : mText(p.mText), mStyle(p.mStyle) {}
        Part(Part &&p) : mText(std::move(p.mText)), mStyle(p.mStyle) {}
        ~Part() {}
        Part &operator = (const Part &p) { mText = p.mText; mStyle = p.mStyle; return *this; }
        Part &operator = (Part &&p) { mText = std::move(p.mText); mStyle = p.mStyle; return *this; }

        std::string mText;
        unsigned mStyle;
    };

    std::vector<Part> mParts;
    int mNumber;
    int mStyle;
};

template <typename Container, typename Func>
void MessageBuilder::appendJoin(Container &&c, Func &&f)
{
    size_t left = c.size();
    for (const auto &v : c) {
        this->append(f(v));
        --left;
        if (left > 1) {
            this->append(", ");
        }
        else if (left == 1) {
            this->append(" and ");
        }
    }
}





#endif // MESSAGEBUILDER_H
