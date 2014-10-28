#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include "jsonserializable.h"
#include ""
class Player : public JsonSerializable {
	public:
		Player(const std::string &name);
		const std::string &name() const { return mName; }
		bool passwordMatch(const std::string &password) const;
		void setPassword(const std::string &password);

		Json::Value serialize() const;
		bool deserialize(const Json::Value &val);
	private:
		std::string hashPassword(const std::string &password);
		std::string mPasswordHash;
		std::string mName;
		RHandle<
};
#endif // PLAYER_H
