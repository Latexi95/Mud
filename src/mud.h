#ifndef MUD_H
#define MUD_H
#include "maineventqueue.h"
#include "room.h"
#include <chrono>

class CommandService;
class PlayerService;
class ResourceService;
class CharacterService;
class LevelService;
class EditorService;

class Mud {
public:
    Mud();
    ~Mud();

    void start();

    MainEventQueue &mainEventQueue() { return mEventQueue; }

    bool loadResources();
    void shutdown();
    time_type time() const;
    time_type timeAfter(float seconds) const;

private:
    std::unique_ptr<CommandService> mCommandService;
    std::unique_ptr<PlayerService> mPlayerService;
    std::unique_ptr<ResourceService> mResourceService;
    std::unique_ptr<LevelService> mLevelService;
    std::unique_ptr<CharacterService> mCharacterService;
    std::unique_ptr<EditorService> mEditorService;

    MainEventQueue mEventQueue;
    std::chrono::system_clock::time_point mLastUpdate;
    volatile bool mRunning;
    volatile time_type mTime;
};

extern Mud *MUD;

#endif // MUD_H
