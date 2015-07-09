#include "dicerandom.h"
#include "dicesequence.h"
#include <vector>

DiceRandom::DiceRandom() {
    std::random_device randomDevice;
    mRandom.seed(randomDevice());
}

DiceRandom::~DiceRandom()
{

}

int DiceRandom::roll(const std::string &input) {
    DiceSequence seq = DiceSequence::fromText(input);
    return roll(seq);
}

int DiceRandom::roll(const DiceSequence &sequence) {
    int total = sequence.mConstantPart;
    for (std::map<DiceSequence::DiceT, DiceSequence::TimesT>::const_iterator i = sequence.mParts.begin(); i != sequence.mParts.end(); i++) {
        total += roll(i->second, i->first);
    }
    return total;
}

int DiceRandom::roll(int times, int d) {
    std::uniform_int_distribution<int> dist(1, d);
    int total = 0;
    for (int i = 0; i < times; i++) {
        total += dist(mRandom);
    }
    return total;
}

int DiceRandom::roll(int d) {
    std::uniform_int_distribution<int> dist(1, d);
    return dist(mRandom);
}
