#pragma once

BEGIN(Engine)

class IPoolBase { public: virtual ~IPoolBase() = default; };

template<typename T, size_t Chunk = 64, bool ThreadSafe = false>
class ObjectPool : public IPoolBase
{
	struct DummyLock { void lock() {} void unlock() {} };
	using  MutexType = conditional_t<ThreadSafe, mutex, DummyLock>;
	using  LockGuard = lock_guard<MutexType>;

public:
	ObjectPool(size_t reserve = 64) { Reserve(reserve); }
	~ObjectPool() { assert(liveCount == 0 && "Object leak detected!"); }

	template<typename... Args>
	T* Acquire(Args&&...  args)
	{
		LockGuard g(lock);

		T* obj = nullptr;
		if (pool.empty())
			obj = T::Create(std::forward<Args>(args)...);
		else
		{
			obj = pool.top();
			pool.pop();
			new (obj) T(std::forward<Args>(args)...);
		}

		if constexpr (requires(T * o, Args&&... a) { o->Ready_Object(std::forward<Args>(a)...); })
			obj->Ready_Object(std::forward<Args>(args)...);

		++liveCount;
		return obj;
	}

	void Release(T* obj)
	{
		if (!obj) return;

		if constexpr (requires(T & o) { o.Reset(); })
			obj->Reset();

		LockGuard g(lock);
		pool.push(obj);
		--liveCount;
	}

	void Reserve(size_t count)
	{
		LockGuard g(lock);
		while (pool.size() < count)
			pool.push(reinterpret_cast<T*>(::operator new(sizeof(T))));
	}

	size_t Alive() const { return liveCount; }

private:
	stack<T*> pool;
	size_t liveCount = 0;
	MutexType lock;
};

END