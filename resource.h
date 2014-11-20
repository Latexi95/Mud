#ifndef RESOURCE_H
#define RESOURCE_H
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>
#include <boost/thread/condition_variable.hpp>
#include <unordered_map>
#include <boost/atomic/fences.hpp>
#include <boost/atomic.hpp>
template<typename T>
class RHandle;

template <typename T>
RHandle<T> createDynamicResource();


class Player;
template<typename T>
class Resource {
		friend class RHandle<T>;
		friend RHandle<T> createDynamicResource<>();
	public:
		Resource(const std::string &path) : mPath(path), mResource(new T()), mRefCount(0) {}
		Resource(const std::string &path, T *r) : mResource(r), mPath(path), mRefCount(0) {}
		~Resource() { delete mResource; }

		T &resource() { return *mResource; }
		const T &resource() const { return *mResource; }
		bool canBeDeleted() const {
			return mRefCount.load(boost::memory_order_consume) == 0;
		}
		const std::string &path() const { return mPath; }
	private:
		Resource() : mResource(new T()), mRefCount(0) {}
		void increaseRefCount() {
			mRefCount.fetch_add(1, boost::memory_order_relaxed);
		}
		bool decreaseRefCount() {
			return mRefCount.fetch_add(-1, boost::memory_order_relaxed) == 1;
		}

		T *mResource;
		const std::string mPath;
		boost::atomic_int mRefCount;
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

		RHandle<T> &operator=(const RHandle<T> &o) {
			if (mResource && mResource->decreaseRefCount() && mResource->path().empty()) {
				boost::atomic_thread_fence(boost::memory_order_acquire);
				delete mResource;
			}
			mResource = o.mResource;
			if (mResource) mResource->increaseRefCount();
		}

		bool isNull() const { return mResource == 0; }
		bool isDynamicResource() const { return mResource->path().empty(); }
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
		Resource<T> *mResource;
};
template <typename T>
RHandle<T> createDynamicResource() {
	Resource<T> *resource = new Resource<T>();
	return RHandle<T>(resource);
}

template<typename T>
class ResourceStash {
	public:
		ResourceStash() {
		}
		~ResourceStash() {
			boost::lock_guard<boost::mutex> lock(mMutex);
			for (typename std::unordered_map<std::string, Resource<T> *>::const_iterator i = mStash.begin(); i != mStash.end(); i++) {
				delete i->second;
			}
		}

		template <typename Loader>
		RHandle<T> get(const std::string &path, Loader loader) {
			{
				boost::lock_guard<boost::mutex> lock(mMutex);
				auto i = mStash.find(path);
				if (i != mStash.end() && i->second != 0) {
					return RHandle<T>(i->second);
				}
			}
			{
				boost::unique_lock<boost::mutex> lock(mMutex);
				auto i = mStash.find(path);
				if (i != mStash.end()) {
					while (i->second == 0) {
						mCondition.wait(lock);
						i = mStash.find(path);
						if (i == mStash.end()) {
							return RHandle<T>();
						}
					}
					return RHandle<T>(i->second);
				}
				mStash[path] = 0;
			}
			Resource<T> *resource = loader(path);
			{
				boost::lock_guard<boost::mutex> lock(mMutex);
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

		RHandle<T> add(Resource<T> *resource) {
			{
				boost::lock_guard<boost::mutex> lock(mMutex);
				if (resource) {
					mStash[resource->path()] = resource;
				}
			}
			mCondition.notify_all();
			return RHandle<T>(resource);
		}

		RHandle<T> create(const std::string &path) {
			Resource<T> *resource = new Resource<T>(path, new T());
			return add(resource);
		}

	private:
		boost::mutex mMutex;
		std::unordered_map<std::string, Resource<T> *> mStash;
		boost::condition_variable mCondition;
};

#endif // RESOURCE_H

