#include "position.h"

Position::Position() :
    mX(0),
    mY(0)
{
}

Position::Position(int x, int y) :
    mX(x),
    mY(y) {

}

int Position::x() const {
    return mX;
}

int Position::y() const {
    return mY;
}

bool Position::operator ==(const Position &p) const {
    return this->mX == p.mX && this->mY == p.mY;
}
