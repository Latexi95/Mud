#ifndef NAME_H
#define NAME_H
#include <string>
#include "jsonserializable.h"
class Name : public JsonSerializable{
	public:
		const static int UnspesifiedItemQuantity = -1;
		enum Flags {
			UseArticleAn = 1,
			ProperNoun = 2
		};

		Name(const std::string &base, int flags = 0);
		Name(const std::string &base, const std::string &pluralForm, int flags = 0);

		~Name();

		const std::string &baseName() const;
		const std::string &pluralForm() const;
		const std::string &undefiniteArticle() const;
		const std::string &definiteArticle() const;

		bool isLike(const std::string &name) const;

		std::string name(int itemQuantity, bool definite) const;

		Json::Value serialize() const;
		bool deserialize(const Json::Value &val);
	private:
		std::string mBase;
		std::string mPluralForm;
		int mFlags;
};

#endif // NAME_H
