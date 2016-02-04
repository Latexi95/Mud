#include "dicesequence.h"
#include <cassert>

DiceSequence::DiceSequence() :
    mConstantPart(0) {
}

DiceSequence::~DiceSequence() {

}

DiceSequence DiceSequence::fromText(const std::string &input) {
    DiceSequence sequence;
    int total = 0;
    std::string num;
    int timesD = 0;
    bool minus = false;
    for (char c : input) {
        if (isspace(c)) continue;
        if (isdigit(c)) {
            num += c;
            continue;
        }
        if (c == 'd') {
            timesD = std::stoi(num);
            if (!timesD) return DiceSequence();
            num.clear();
            continue;
        }
        if (c == '+') {
            int n = std::stoi(num);
            if (!n) return DiceSequence();
            num.clear();
            if (timesD) {
                assert(!minus && "Negative dice rolls are not supported");
                sequence.add(timesD, n);
                timesD = 0;
            }
            else {
                total += minus ? -n : n;
            }
            continue;
        }
        if (c == '-') {
            if (num.empty()) {
                minus = !minus;
            }
            else {
                int n = std::stoi(num);
                if (!n) return DiceSequence();
                num.clear();
                if (timesD) {
                    assert(!minus && "Negative dice rolls are not supported");
                    sequence.add(timesD, n);
                    timesD = 0;
                }
                else {
                    total += minus ? -n : n;
                }
                minus = true;
            }
            continue;
        }
    }
    int n = std::stoi(num);
    if (!n) return DiceSequence();
    if (timesD) {
        assert(!minus && "Negative dice rolls are not supported");
        sequence.add(timesD, n);
    }
    else {
        total += minus ? -n : n;
    }
    sequence.setConstantPart(total);
    return sequence;
}

void DiceSequence::add(int times, int d) {
    mParts[d] += times;
}

void DiceSequence::add(int c) {
    mConstantPart += c;
}

double DiceSequence::expectationValue() const {
    double e = mConstantPart;
    for (std::map<DiceT, TimesT>::const_iterator i = mParts.begin(); i != mParts.end(); i++) {
        e += i->second * ((i->first + 1)/ 2.0);
    }
    return e;
}

int DiceSequence::minValue() const {
    int total = mConstantPart;
    for (std::map<DiceT, TimesT>::const_iterator i = mParts.begin(); i != mParts.end(); i++) {
        total += i->second;
    }
    return total;
}

int DiceSequence::maxValue() const {
    int total = mConstantPart;
    for (std::map<DiceT, TimesT>::const_iterator i = mParts.begin(); i != mParts.end(); i++) {
        total += i->second * i->first;
    }
    return total;
}

bool DiceSequence::isNull() {
    return !mConstantPart && mParts.empty();
}

std::string DiceSequence::toText(bool tight) const {
    std::string text;
    std::map<DiceT, TimesT>::const_iterator i = mParts.begin();
    while(i != mParts.end()) {
        if (!text.empty()) {
            text += tight ? "+" : " + ";
        }
        text += std::to_string(i->second) + 'd' +  std::to_string(i->first);
        i++;
    }
    if (mConstantPart) {
        if (mConstantPart < 0) {
            text += tight ? "-" : " - " +  std::to_string(-mConstantPart);
        }
        else {
            text += tight ? "+" :" + " +  std::to_string(mConstantPart);
        }
    }
    return text;
}

Json::Value Json::Serializer<DiceSequence>::serialize(const DiceSequence &d)
{
    return d.toText(true);
}

void Json::Serializer<DiceSequence>::deserialize(const Json::Value &v, DiceSequence &d)
{
    if (v.isString()) {
        d = DiceSequence::fromText(v.asString());
    }
    else if (v.isInt()) {
        d.mParts.clear();
        d.mConstantPart = v.asInt();
    }
    else {
        throw SerializationException("Invalid dice sequence");
    }
}
