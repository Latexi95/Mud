#ifndef CHARACTERSERVICE_H
#define CHARACTERSERVICE_H
#include <memory>
#include "character.h"
#include <boost/thread/mutex.hpp>


class CharacterService {
	public:
		CharacterService();
		~CharacterService();
		static CharacterService *instance();

		std::shared_ptr<Character> character(const std::string &characterId);
		std::shared_ptr<Character> createCharacter(const std::string &characterId);
		void saveCharacter(const std::shared_ptr<Character> &c);
	private:
		boost::mutex mMutex;
		std::unordered_map<std::string, std::shared_ptr<Character> > mCharacters;

};

extern CharacterService * CS;

#endif // CHARACTERSERVICE_H
