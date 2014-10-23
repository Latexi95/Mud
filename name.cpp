#include "name.h"
#include <boost/locale.hpp>
#include <boost/lexical_cast.hpp>
static std::string sArticleA("a");
static std::string sArticleAn("an");
static std::string sArticleThe("the");

Name::Name(const std::string &base, int flags) :
	mBase(base),
	mFlags(flags) {

}

Name::Name(const std::string &base, const std::string &pluralForm, int flags) :
	mBase(base),
	mPluralForm(pluralForm),
	mFlags(flags) {

}



const std::string &Name::baseName() const {
	return mBase;
}

const std::string &Name::pluralForm() const {
	if (mPluralForm.empty()) {
		mPluralForm = mBase;
		mPluralForm+= 's';
	}
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
	std::string nameLower = boost::locale::to_lower(name);
	if (mBase == nameLower  || mPluralForm = nameLower) {
		return true;
	}

	return false;
}

std::string Name::name(int itemQuantity, bool definite) const {
	if (itemQuantity == 1) {
		if (definite || (mFlags & ProperNoun) ) {
			return definiteArticle() + " " +
		}
	}
}
