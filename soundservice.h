#ifndef SOUNDSERVICE_H
#define SOUNDSERVICE_H
#include "level.h"

enum class SoundLevel {
    SL0 = 0,
    SL1,
    SL2,
    SL3,
    SL4,

    Inaudible = SL0,
    Whisper = SL1,
    Normal = SL2,
    Shout = SL3,
    Explosion = SL4
};

class SoundService
{
public:
    SoundService();
    ~SoundService();
    static SoundService *instance();

    std::vector<std::shared_ptr<Character> > charactersWhoCanHearSound(const Position &soundPos, const std::shared_ptr<Level> &level, bool limitToLevel);

};

extern SoundService *SS;

#endif // SOUNDSERVICE_H
