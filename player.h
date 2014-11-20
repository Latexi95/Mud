#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include "jsonserializable.h"
#include "character.h"
class Player : public JsonSerializable {
	public:
		Player();
		Player(const std::string &name);
		const std::string &name() const { return mName; }
		void setName(const std::string &name);
		bool passwordMatch(const std::string &password) const;
		void setPassword(const std::string &password);

		Json::Value serialize() const;
		bool deserialize(const Json::Value &val);
		bool isLoggedIn() const;
		void setLoggedIn(bool loggedIn);
	private:
		std::string hashPassword(const std::string &password) const;
		std::string mPasswordHash;
		std::string mName;
		bool mLoggedIn;
		RHandle<Character> mCharacter;
};
#endif // PLAYER_H
