#ifndef LEVELSERVICE_H
#define LEVELSERVICE_H
#include "level.h"
#include <memory>
#include <boost/thread/mutex.hpp>

class LevelService {
public:
    LevelService();
    ~LevelService();
    static LevelService *instance();

    Level *level(const std::string &levelId);

    bool loadAllLevels();
    std::shared_ptr<Level> loadLevel(const std::string &levelId, bool directAdd = false);
private:
    std::shared_ptr<Level> loadLevelNoLock(const std::string &levelId, bool directAdd = false);
    boost::mutex mMutex;
    std::unordered_map<std::string, std::shared_ptr<Level> > mLevels;
};
extern LevelService *LS;
#endif // LEVELSERVICE_H
