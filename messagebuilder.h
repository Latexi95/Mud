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

    static bool underlined(int style);
    static bool bolded(int style);
    static bool foregroundColorSet(int style);
    static bool backgroundColorSet(int style);
    static bool styleNoSpace(int style);

    void setStyle(int style);
    int style() const;

    MessageBuilder &operator<< (int i);
    MessageBuilder &operator<< (const std::string &str);
    MessageBuilder &operator<< (std::string &&str);
    MessageBuilder &operator<< (const Name &name);
    MessageBuilder &operator<< (const std::unique_ptr<Item> &item);
    MessageBuilder &operator<< (const std::shared_ptr<Character> &character);

    MessageBuilder &operator<< (const SetStyle &setStyle);
    MessageBuilder &operator<< (Style xorStyle);

    template <size_t S>
    MessageBuilder &operator<< (char text[S]) {
        append(std::string(text, S));
        return *this;
    }
private:
    static char foregroundTelnetColorCode(int style);
    static char backgroundTelnetColorCode(int style);

    void clearNumberStash();
    struct Part {
        Part(int style, const std::string &txt) : mText(txt), mStyle(style) {}
        Part(int style, std::string &&txt) : mText(std::move(txt)), mStyle(style) {}
        Part(const Part &p) : mText(p.mText), mStyle(p.mStyle) {}
        Part(Part &&p) : mText(std::move(p.mText)), mStyle(p.mStyle) {}
        ~Part() {}
        Part &operator = (const Part &p) { mText = p.mText; mStyle = p.mStyle; return *this; }
        Part &operator = (Part &&p) { mText = std::move(p.mText); mStyle = p.mStyle; return *this; }

        std::string mText;
        int mStyle;
    };

    std::vector<Part> mParts;
    int mNumber;
    int mStyle;
};





#endif // MESSAGEBUILDER_H
