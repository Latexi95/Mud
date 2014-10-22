#include "jsonchecker.h"

JsonChecker::JsonChecker(const Json::Value &val, const char *path) :
	mPath(path),
	mIsValid(true)
{
	mValueStack.push(&val);
}

JsonChecker &JsonChecker::moveTo(const char *obj, bool optional) {
	if (!mIsValid) return *this;
	Json::Value *n = current();
	if (n == 0) {
		mValueStack.push(0);
		mJsonPath.push(std::string());
		return *this;
	}
	const Json::Value &next = *n[obj];
	if (next.isNull()) {
		if (optional) {
			mValueStack.push(0);
			mJsonPath.push(std::string());
			return *this;
		}
		mErrorMessage = "Failed to load file \"" + std::string(mPath) + "\" : Can't find key \"" + mJsonPath + obj + "\"";
		mIsValid = false;
		return *this;
	}
	if (!next.isObject()) {
		mErrorMessage = "Failed to load file \"" + std::string(mPath) + "\" : Value of the key \"" + mJsonPath + obj + "\" is not an object";
		mIsValid = false;
		return *this;
	}

	mValueStack.push(&next);
	mJsonPath.push(obj);
	return *this;
}

JsonChecker &JsonChecker::moveBack() {
	mValueStack.pop();
	mJsonPath.pop();
}

JsonChecker &JsonChecker::hasNumberElement(const char *obj, bool optional) {
	if (!mIsValid) return *this;
	const Json::Value &num = current()[obj];
	if (num.isNull()) {
		if (optional)
			return *this;
		mErrorMessage = "Failed to load file \"" + std::string(mPath) + "\" : Can't find key \"" + joinJsonPath() + obj + "\"";
		mIsValid = false;
		return *this;
	}
	if (!num.isNumeric()) {
		mErrorMessage = "Failed to load file \"" + std::string(mPath) + "\" : Value of the key \"" + joinJsonPath() + obj + "\" is not a number";
		mIsValid = false;
		return *this;
	}
	return *this;
}

JsonChecker &JsonChecker::hasStringElement(const char *obj, bool optional) {
	if (!mIsValid) return *this;
	const Json::Value &string = current()[obj];
	if (string.isNull()) {
		if (optional)
			return *this;
		mErrorMessage = "Failed to load file \"" + std::string(mPath) + "\" : Can't find key \"" + joinJsonPath() + obj + "\"";
		mIsValid = false;
		return *this;
	}
	if (!string.isString()) {
		mErrorMessage = "Failed to load file \"" + std::string(mPath) + "\" : Value of the key \"" + joinJsonPath() + obj + "\" is not a string";
		mIsValid = false;
		return *this;
	}
	return *this;
}

JsonChecker &JsonChecker::hasBooleanElement(const char *obj, bool optional) {
	if (!mIsValid) return *this;
	const Json::Value &num = current()[obj];
	if (num.isNull()) {
		if (optional)
			return *this;
		mErrorMessage = "Failed to load file \"" + std::string(mPath) + "\" : Can't find key \"" + joinJsonPath() + obj + "\"";
		mIsValid = false;
		return *this;
	}
	if (!num.isBool()) {
		mErrorMessage = "Failed to load file \"" + std::string(mPath) + "\" : Value of the key \"" + joinJsonPath() + obj + "\" is not a boolean";
		mIsValid = false;
		return *this;
	}
	return *this;
}

JsonChecker &JsonChecker::hasObjectElement(const char *obj, bool optional) {
	if (!mIsValid) return *this;
	const Json::Value &num = current()[obj];
	if (num.isNull()) {
		if (optional)
			return *this;
		mErrorMessage = "Failed to load file \"" + std::string(mPath) + "\" : Can't find key \"" + joinJsonPath() + obj + "\"";
		mIsValid = false;
		return *this;
	}
	if (!num.isObject()) {
		mErrorMessage = "Failed to load file \"" + std::string(mPath) + "\" : Value of the key \"" + joinJsonPath() + obj + "\" is not an object";
		mIsValid = false;
		return *this;
	}
	return *this;
}

JsonChecker &JsonChecker::hasStringListElement(const char *obj, bool optional) {
	if (!mIsValid) return *this;
	const Json::Value &num = current()[obj];
	if (num.isNull()) {
		if (optional)
			return *this;
		mErrorMessage = "Failed to load file \"" + std::string(mPath) + "\" : Can't find key \"" + joinJsonPath() + obj + "\"";
		mIsValid = false;
		return *this;
	}
	if (!num.isArray()) {
		mErrorMessage = "Failed to load file \"" + std::string(mPath) + "\" : Value of the key \"" + joinJsonPath() + obj + "\" is not an array";
		mIsValid = false;
		return *this;
	}
	for (Json::Value::const_iterator i = num.begin(); i != num.end(); i++) {
		if (!i->isString()) {
			mErrorMessage = "Failed to load file \"" + std::string(mPath) + "\" : Value of the key \"" + joinJsonPath() + obj + "\" is not a string array";
		}
	}

	return *this;
}

const Json::Value *JsonChecker::current() const {
	assert(!mValueStack.empty());
	return mValueStack.top();
}

std::string JsonChecker::joinJsonPath() const {
	std::string s;
	for (const std::string &v : mJsonPath) {
		s += v + '/';
	}
	return s;
}

JsonChecker::operator bool() const {
	return mIsValid;
}



