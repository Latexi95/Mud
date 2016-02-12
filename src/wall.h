#ifndef WALL_H
#define WALL_H
#include "item.h"
#include "character.h"

#include "enums.h"
#include "roomexit.h"
#include "jsonserializable.h"

class Wall {
    template<typename T>
    friend struct Json::Serializer;
public:
    Wall();
    ~Wall();
    const std::vector<std::unique_ptr<Item> > &items() const;
    void addItem(std::unique_ptr<Item> &&i);

    const std::string &looks() const;
    void setLooks(const std::string &looks);

    RoomExit *exit() const;
    void setExit(std::unique_ptr<RoomExit> &&e);
    bool hasExit() const;
protected:
    std::string mLooks;
    std::vector<std::unique_ptr<Item> > mItems;
    std::unique_ptr<RoomExit> mExit;

};


namespace Json {
template<>
struct Serializer<Wall> {
    static Value serialize(const Wall &w);
    static void deserialize(const Value &v, Wall &w);
};
}


#endif // WALL_H
