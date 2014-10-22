#ifndef JSONSERIALIZABLE_H
#define JSONSERIALIZABLE_H
#include <json/value.h>
class JsonSerializable {
	public:
		JsonSerializable();
		virtual ~JsonSerializable();
		virtual Json::Value serialize() const = 0;
		virtual bool deserialize(const Json::Value &val) = 0;
		static std::string valueAsString(const Json::Value &val, bool &success);
	protected:

};

#endif // JSONSERIALIZABLE_H
