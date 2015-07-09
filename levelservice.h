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

    std::shared_ptr<Level> level(const std::string &levelId);
private:
    boost::mutex mMutex;
    std::unordered_map<std::string, std::shared_ptr<Level> > mLevels;
};
extern LevelService *LS;
#endif // LEVELSERVICE_H
