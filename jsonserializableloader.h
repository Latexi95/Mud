#ifndef JSONSERIALIZABLELOADER_H
#define JSONSERIALIZABLELOADER_H
#include "jsonserializable.h"
#include "resource.h"
#include "resourceservice.h"
#include <boost/make_unique.hpp>
template <typename T>
class JsonSerializableLoader {
	public:
		Resource<T> *operator()(const std::string &path) const;
};

template <typename T>
inline Resource<T> *JsonSerializableLoader<T>::operator()(const std::string &path) const {
	Json::Value v = ResourceService::instance()->requestJsonResource(path);
	if (v.isNull()) return 0;
	std::unique_ptr<T> t = boost::make_unique<T>();
	if (!t->deserialize(v)) {
		return 0;
	}
	return new Resource<T>(path, t.release());
}

#endif // JSONSERIALIZABLELOADER_H
