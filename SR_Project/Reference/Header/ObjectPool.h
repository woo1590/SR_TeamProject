#pragma once

BEGIN(Engine)

template<typename T>
struct RawBlock
{
	alignas(T) std::byte data[sizeof(T)];
};

template<typename T, size_t Chunck = 64, bool ThreadSafe = false>
class ObjectPool
{
	using Storage = RawBlock<T>;

public:
	~ObjectPool()
	{
		assert(freeList.size() == storage.size() && "Leaked objects in pool");
	}

public:
	template<typename... Args>
	[[nodiscard]] T* Acquire(Args&&... args)
	{
		LockGuard g(lock);
		if (freeList.empty())
			Grow();

		size_t idx = freeList.top(); 
		freeList.pop();
		T* ptr = new (&storage[idx]) T(forward<Args>(args)...);
		return ptr;
	}

	void Release(T* obj)
	{
		if (!obj) return;
		obj->~T();

		LockGuard g(lock);
		size_t idx = Index(obj);
		assert(idx < storage.size() && "Invalid pointer release");
		freeList.push(idx);
	}

	void Reserve(size_t n)
	{
		LockGuard g(lock);
		if (n > storage.size())
			Grow(n - storage.size());
	}

private:
	void Grow(size_t extra = Chunck)
	{
		size_t old = storage.size();
		storage.resize(old + extra);
		for (size_t i = 0; i < extra; ++i)
			freeList.push(old + i);
	}

	size_t Index(T* p) const noexcept
	{
		return reinterpret_cast<Storage*>(p) - storage.data();
	}

private:
	struct DummyLock { void lock() {} void unlock() {} };
	using MutexType = conditional_t<ThreadSafe, mutex, DummyLock>;
	using LockGuard = lock_guard<MutexType>;

	vector<Storage> storage;
	stack<size_t> freeList;
	MutexType lock;
};

END