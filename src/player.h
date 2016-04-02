#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include "util/jsonserializable.h"
#include "character.h"
class Player {
public:
    Player();
    Player(const std::string &name);
    const std::string &name() const { return mName; }
    void setName(const std::string &name);
    bool passwordMatch(const std::string &password) const;
    void setPassword(const std::string &password);

    Json::Value serialize() const;
    void deserialize(const Json::Value &val);
    bool isLoggedIn() const;
    void setLoggedIn(bool loggedIn);

    bool isComplete() const;

    const std::vector<std::string> &characterNames() const;
    void addCharacterName(const std::string &name);
    std::shared_ptr<Character> currentCharacter() const;

private:
    std::string hashPassword(const std::string &password) const;
    std::string mPasswordHash;
    std::string mName;
    bool mLoggedIn;
    std::vector<std::string> mCharacterNames;
    std::shared_ptr<Character> mCurrentCharacter;
};

namespace Json {
template<>
struct Serializer<Player> {
    static Value serialize(const Player &p);
    static void deserialize(const Value &v, Player &p);
};
}

#endif // PLAYER_H
