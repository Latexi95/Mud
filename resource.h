#ifndef RESOURCE_H
#define RESOURCE_H
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>
#include <boost/thread/condition_variable.hpp>
#include <unordered_map>
#include <boost/atomic/fences.hpp>

template<typename T>
class RHandle;

class Player;
template<typename T>
class Resource {
		friend class RHandle<T>;
		template <typename T>
		friend RHandle<T> createStaticResource();
	public:
		Resource(const std::string &path) : mPath(path), mRefCount(0) {}
		Resource(const std::string &path, const T &r) : mResource(r), mPath(path), mRefCount(0) {}
		T &resource() { return mResource; }
		const T &resource() const { return mResource; }
		bool canBeDeleted() const {
			return mRefCount.load(boost::memory_order_consume);
		}
		const std::string &path() const { return mPath; }
	private:
		Resource() : mRefCount(0) {}
		void increaseRefCount() {
			mRefCount.fetch_add(1, boost::memory_order_relaxed);
		}
		bool decreaseRefCount() {
			return mRefCount.fetch_add(-1, boost::memory_order_relaxed) == 1;
		}

		T mResource;
		const std::string mPath;
		boost::atomic_int64_t mRefCount;
};

template<typename T>
class RHandle {
	public:
		RHandle() : mResource(0) {}
		RHandle(Resource<T> *r) : mResource(r) {
			if (mResource) mResource->increaseRefCount();
		}
		RHandle(const RHandle<T> &handle) : mResource(handle.mResource){
			if (mResource) mResource->increaseRefCount();
		}
		~RHandle() {
			if (mResource && mResource->decreaseRefCount() && mResource->path().empty()) {
				boost::atomic_thread_fence(boost::memory_order_acquire);
				delete mResource;
			}
		}

		bool isNull() const { return mResource == 0; }
		T *operator ->() const {
			assert(mResource);
			return &mResource->resource();
		}
		T &operator*() const {
			assert(mResource);
			return mResource->resource();
		}
		const std::string &path() const {
			assert(mResource);
			return mResource->path();
		}
	private:
		Resource *mResource;
};
template <typename T>
RHandle<T> createStaticResource() {
	Resource<T> *resource = new Resource<T>();
	return RHandle<T>(resource);
}

template<typename T, typename Loader >
class ResourceStash {
	public:
		ResourceStash(Loader loader) : mLoader(loader) {
		}
		~ResourceStash() {
			boost::lock_guard lock(&mMutex);
			for (std::unordered_map<std::string, Resource<T> *>::const_iterator i = mStash.begin(); i != mStash.end(); i++) {
				delete i->second;
			}
		}

		RHandle get(const std::string &path);

	private:
		Loader mLoader;
		boost::mutex mMutex;
		std::unordered_map<std::string, Resource<T> *> mStash;
		boost::condition_variable mCondition;
};

#endif // RESOURCE_H


template<typename T, typename Loader >
RHandle ResourceStash::get(const std::string &path) {
	{
		boost::lock_guard lock(&mMutex);
		auto i = mStash.find(path);
		if (i != mStash.end() && i->second() != 0) {
			return RHandle<T>(i->second());
		}
	}
	{
		boost::unique_lock<boost::mutex> lock(&mMutex);
		auto i = mStash.find(path);
		if (i != mStash.end()) {
			while (i->second() == 0) {
				mCondition.wait(lock);
				i = mStash.find(path);
				if (i == mStash.end()) {
					return RHandle<T>();
				}
			}
			return RHandle<T>(i->second());
		}
		mStash[path] = 0;
	}
	Resource<T> *resource = mLoader(path);
	{
		boost::lock_guard lock(&mMutex);
		if (resource) {
			mStash[path] = resource;
		}
		else {
			mStash.erase(path);
			return RHandle<T>();
		}
	}
	mCondition.notify_all();
	return RHandle<T>(resource);
}
