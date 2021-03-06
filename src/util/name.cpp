#include "util/name.h"
#include <boost/locale.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include "util/stringbuilder.h"
#include "util/textutils.h"

static std::string sArticleA("a");
static std::string sArticleAn("an");
static std::string sArticleThe("the");

Name::Name() :
    mFlags(0) {

}

Name::Name(const std::string &base, int flags) :
    mBase(base),
    mFlags(flags) {

}

Name::Name(const std::string &base, const std::string &pluralForm, int flags) :
    mBase(base),
    mPluralForm(pluralForm),
    mFlags(flags) {

}

Name::~Name() {

}



const std::string &Name::baseName() const {
    return mBase;
}

std::string Name::pluralForm() const {
    if (mPluralForm.empty()) {
        if (boost::ends_with(mBase, "s") || boost::ends_with(mBase, "x") || boost::ends_with(mBase, "ch")) {
            return mBase + "es";
        }
        return mBase + 's';
    }
    return mPluralForm;
}

const std::string &Name::undefiniteArticle() const {
    if (mFlags & UseArticleAn) {
        return sArticleAn;
    }
    return sArticleA;
}

const std::string &Name::definiteArticle() const {
    return sArticleThe;
}

bool Name::isLike(const std::string &name) const {
    boost::locale::comparator<char,boost::locale::collator_base::secondary> cmpr;
    if (cmpr(name, mBase) || cmpr(name, pluralForm())) {
        return true;
    }

    return false;
}

std::string Name::num(int itemQuantity, bool definite) const {
    if (itemQuantity == 1) {
        if (definite || (mFlags & ProperNoun) ) {
            return SB(definiteArticle()) + ' ' + mBase;
        } else {
            return SB(undefiniteArticle()) + ' ' + mBase;
        }
    }
    else if (itemQuantity == 0) {
        return SB("no ") + pluralForm();
    }
    else if (itemQuantity > 1) {
        return SBInt(itemQuantity) + ' ' + pluralForm();
    }
    else {
        if (definite) {
            return SB(definiteArticle()) + pluralForm();
        }
        else {
            return pluralForm();
        }
    }
}

Json::Value Name::serialize() const {
    if (mPluralForm.empty()) {
        if (mFlags == NoFlags) {
            return Json::Value(mBase);
        }
        else {
            Json::Value obj(Json::objectValue);
            obj["base"] = mBase;
            obj["flags"] = mFlags;
            return obj;
        }
    }
    Json::Value obj(Json::objectValue);
    obj["base"] = mBase;
    obj["plural"] = mBase;
    obj["flags"] = mFlags;
    return obj;
}

void Name::deserialize(const Json::Value &val) {
    if (val.isString()) {
        mBase = val.asString();
        mPluralForm.clear();
        mFlags = NoFlags;
        return;
    }
    if (val.isObject()) {
        const Json::Value &base = val["base"];
        const Json::Value &plural = val["plural"];
        const Json::Value &flags = val["flags"];
        if (base.isString()) {
            mBase = base.asString();
        }
        if (plural.isString()) {
            mPluralForm = base.asString();
        }
        if (flags.isInt()) {
            int flagsInt = flags.asInt();
            mFlags = flagsInt & MaskFlag;
        }
        if (mBase.empty()) {
            throw SerializationException("Expecting a base name for Name");
        }
    }
    throw SerializationException("Expecting a name (a string or an object)");
}

bool Name::isNull() const {
    return mBase.empty();
}

bool Name::operator==(const Name &n) const {
    return this->mBase == n.mBase;
}

bool Name::operator!=(const Name &n) const {
    return this->mBase != n.mBase;
}

bool Name::operator<(const Name &n) const {
    return this->mBase < n.mBase;
}



Name Name::fromParseString(std::string s)
{
    text::clean(s);
    if (s.empty()) return Name();
    std::string lower = boost::locale::to_lower(s);

    int flags = NoFlags;
    if (boost::starts_with(lower, "the ")) {
        flags |= ProperNoun;
        s = s.substr(4);
    }
    else if (boost::starts_with(lower, "a ")) {
        s = s.substr(2);
    }
    else if (boost::starts_with(lower, "an ")){
        flags |= UseArticleAn;
        s = s.substr(3);
    } else {
        switch (lower[0]) {
            case 'a':
            case 'e':
            case 'i':
            case 'o':
            case 'u':
            flags |= UseArticleAn;
            break;
        default:
            break;
        }
    }

    std::string::size_type splitIndex = s.find("/");
    if (splitIndex != std::string::npos) {
        std::string base = s.substr(0, splitIndex);
        std::string plural = s.substr(splitIndex + 1);
        text::clean(plural);
        text::clean(base);
        return Name(base, plural, flags);
    }

    return Name(s, flags);
}

std::string Name::toParseString() const
{
    std::string ret;
    if (mFlags & ProperNoun) {
        ret = "the ";
    } else if (mFlags & UseArticleAn) {
        ret = "an ";
    } else {
        ret = "a ";
    }
    ret += mBase;
    if (!mPluralForm.empty()) {
        ret += '/';
        ret += mPluralForm;
    }
    return ret;
}
