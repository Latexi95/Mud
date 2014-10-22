#ifndef DICERANDOM_H
#define DICERANDOM_H
#include <random>
class DiceSequence;
class DiceRandom {
	public:
		DiceRandom();
		~DiceRandom();
		int roll(const std::string &input);
		int roll(const DiceSequence &sequence);
		int roll(int times, int d);
		int roll(int d);
	private:
		std::mt19937 mRandom;
};

#endif // DICERANDOM_H
