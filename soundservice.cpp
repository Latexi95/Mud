#include "soundservice.h"

SoundService *SS = 0;

SoundService::SoundService()
{
    assert(SS == 0);
    SS = this;
}

SoundService::~SoundService()
{
    assert(SS);
    SS = 0;
}

SoundService *SoundService::instance()
{
    return SS;
}

std::vector<std::shared_ptr<Character> > SoundService::charactersWhoCanHearSound(const Position &soundPos, const std::shared_ptr<Level> &level, bool limitToLevel)
{
    std::vector<std::shared_ptr<Character> > ret;

    return ret;
}

