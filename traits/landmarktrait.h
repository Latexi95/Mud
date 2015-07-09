#ifndef LANDMARKTRAIT_H
#define LANDMARKTRAIT_H
#include "itemtrait.h"

class LandmarkTrait : public ItemTrait {
public:
    LandmarkTrait();
    ~LandmarkTrait();
    Type type() const { return Landmark; }
    std::unique_ptr<ItemTrait> clone() const;
    Json::Value serialize() const;
    void deserialize(const Json::Value &val);
    const char *traitName() const { return "landmark"; }
};

#endif // LANDMARKTRAIT_H
