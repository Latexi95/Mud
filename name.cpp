#include "name.h"
#include <boost/locale.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include "stringbuilder.h"
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

const std::string &Name::pluralForm() const {
	if (mPluralForm.empty()) {
		mPluralForm = mBase;
		mPluralForm+= 's';
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

std::string Name::name(int itemQuantity, bool definite) const {
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
	if (mPluralForm.empty() ||
			(mPluralForm.size() + 1 == mBase.size() && mPluralForm[mPluralForm.size() - 1] == 's' && boost::starts_with(mPluralForm, mBase))) {
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

bool Name::deserialize(const Json::Value &val) {
	if (val.isString()) {
		mBase = val.asString();
		mPluralForm.clear();
		mFlags = NoFlags;
		return true;
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
		if (mBase.empty()) return false;
		return true;
	}
	return false;
}

bool Name::isNull() const {
	return mBase.empty();
}

bool Name::operator==(const Name &n) const {
	return this->mBase == n.mBase && this->mPluralForm == n.mPluralForm && this->mFlags == n.mFlags;
}

bool Name::operator!=(const Name &n) const {
	return this->mBase != n.mBase || this->mPluralForm != n.mPluralForm || this->mFlags != n.mFlags;
}
