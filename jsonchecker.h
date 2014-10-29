#ifndef JSONCHECKER_H
#define JSONCHECKER_H
#include <json/value.h>
#include <stack>
class JsonChecker {
	public:
		JsonChecker(const Json::Value &val, const char *path);
		operator bool () const;


		JsonChecker &moveTo(const char *obj, bool optional);
		JsonChecker &moveBack();
		JsonChecker &hasNumberElement(const char *obj, bool optional = false);
		JsonChecker &hasStringElement(const char *obj, bool optional = false);
		JsonChecker &hasBooleanElement(const char *obj, bool optional = false);
		JsonChecker &hasObjectElement(const char *obj, bool optional = false);
		JsonChecker &hasStringListElement(const char *obj, bool optional = false);
		const std::string &errorMessage() const { return mErrorMessage; }
	private:
		const Json::Value *current() const;
		std::string joinJsonPath() const;
		std::stack<const Json::Value *> mValueStack;
		std::vector<std::string> mJsonPath;
		const char *mPath;
		std::string mErrorMessage;
		bool mIsValid;
};

#endif // JSONCHECKER_H
