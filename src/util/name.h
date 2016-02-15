#ifndef NAME_H
#define NAME_H
#include <string>
#include "jsonserializable.h"
class Name {
public:
    const static int UnspesifiedItemQuantity = -1;
    enum Flags {
        NoFlags = 0,
        UseArticleAn = 1,
        ProperNoun = 2,
        MaskFlag = 3
    };

    Name();
    Name(const std::string &base, int flags = 0);
    Name(const std::string &base, const std::string &pluralForm, int flags = 0);

    ~Name();

    const std::string &baseName() const;
    std::string pluralForm() const;
    const std::string &undefiniteArticle() const;
    const std::string &definiteArticle() const;

    bool isLike(const std::string &name) const;

    std::string num(int itemQuantity, bool definite) const;

    Json::Value serialize() const;
    void deserialize(const Json::Value &val);

    bool isNull() const;

    bool operator==(const Name &n) const;
    bool operator!=(const Name &n) const;
    bool operator<(const Name &n) const;

    static Name fromParseString(std::string s);
    std::string toParseString() const;
private:
    std::string mBase;
    std::string mPluralForm;
    int mFlags;
};


#endif // NAME_H
