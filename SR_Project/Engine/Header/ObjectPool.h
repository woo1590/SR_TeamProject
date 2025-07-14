#pragma once

BEGIN(Engine)

template<typename T>
struct RawBlock { alignas(T) std::byte data[sizeof(T)]; };

class IPoolBase { public: virtual ~IPoolBase() = default; };

template<typename T, size_t Chunk = 64, bool ThreadSafe = false>
class ObjectPool : public IPoolBase
{
	struct DummyLock { void lock() {} void unlock() {} };
	using Storage = RawBlock<T>;
	using  MutexType = conditional_t<ThreadSafe, mutex, DummyLock>;
	using  LockGuard = lock_guard<MutexType>;

public:
	~ObjectPool() { assert(freeList.size() == storage.size() && "Leaked objects in pool"); }

	template<typename... Args>
	[[nodiscard]] T* Acquire(Args&&... args)
	{
		LockGuard g(lock);
		if (freeList.empty()) Grow();

		Storage* cell = freeList.back();
		freeList.pop_back();

		T* obj = new (cell) T(std::forward<Args>(args)...);

		if constexpr (requires(T * p, Args&&... a)
		{
			p->Ready_Object(std::forward<Args>(a)...);
		})
			obj->Ready_Object(std::forward<Args>(args)...);
		if constexpr (requires(T & o) { o.Reset(); })
			obj->Reset();

		return obj;
	}

	void Release(T* obj)
	{
		if (!obj) return;
		if constexpr (requires(T & t) { t.Reset(); })
			obj->Reset();
		obj->~T();

		LockGuard g(lock);
#ifdef DEBUG
		assert(find(freeList.begin(), freeList.end(), cell) == freeList.end() && "Double free");
#endif 

		freeList.push_back(reinterpret_cast<Storage*>(obj));
	}

	void Reserve(size_t n)
	{
		LockGuard g(lock);
		if (n > storage.size())
			Grow(n - storage.size());
	}

	size_t Alive() const { return storage.size() - freeList.size(); }
	size_t Capacity() const { return storage.size(); }

private:
	void Grow(size_t extra = Chunk)
	{
		size_t old = storage.size();
		storage.resize(old + extra);
		for (size_t i = 0; i < extra; ++i)
			freeList.push_back(&storage[old + i]);
	}

	void Shrink()
	{
		LockGuard g(lock);
		if (freeList.size() == storage.size())
		{
			storage.clear();
			freeList.clear();
		}
	}

private:
	
	deque<Storage> storage;
	vector<Storage*> freeList;
	MutexType lock;
};

END