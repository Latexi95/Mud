#ifndef DICESEQUENCE_H
#define DICESEQUENCE_H
#include <map>
#include <string>
#include "jsonserializable.h"
class DiceSequence : public JsonSerializable {
	public:
		friend class DiceRandom;
		DiceSequence();
		~DiceSequence();
		static DiceSequence fromText(const std::string &input);

		void add(int times, int d);
		void add(int c);
		void setConstantPart(int c) { mConstantPart = c; }
		int constantPart() const { return mConstantPart; }
		double expectationValue() const;
		int minValue() const;
		int maxValue() const;
		bool isNull();

		std::string toText(bool tight = false) const;

		Json::Value serialize() const;
		void deserialize(const Json::Value &val);
	public:
		typedef char DiceT;
		typedef char TimesT;

		int mConstantPart;
		std::map<DiceT, TimesT> mParts;
};

#endif // DICESEQUENCE_H
