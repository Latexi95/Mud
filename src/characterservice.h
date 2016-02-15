#ifndef CHARACTERSERVICE_H
#define CHARACTERSERVICE_H
#include <memory>
#include "character.h"
#include <boost/thread/mutex.hpp>
#include "util/enums.h"

class CharacterService {
public:
    CharacterService();
    ~CharacterService();
    static CharacterService *instance();

    std::shared_ptr<Character> character(const std::string &characterId);
    std::shared_ptr<Character> createCharacter(const std::string &characterId);

    bool doesCharacterExits(const std::string &characterId);

    void saveCharacter(const std::shared_ptr<Character> &c);
    bool canMove(const std::shared_ptr<Character> &c, Direction dir);
    void startMove(const std::shared_ptr<Character> &c, Direction dir);

    float travelSpeed(const std::shared_ptr<Character> &r);
    float travelTime(const std::shared_ptr<Character> &r, float distance);

    bool loadAllCharacters();
    std::shared_ptr<Character> loadCharacter(const std::string &fileName);

    std::vector<Item*> itemsInVision(const std::shared_ptr<Character> &c);
private:
    boost::mutex mMutex;
    std::unordered_map<std::string, std::shared_ptr<Character> > mCharacters;
};

extern CharacterService * CS;

#endif // CHARACTERSERVICE_H
