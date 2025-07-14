#pragma once

#include "ObjectPool.h"

BEGIN(Engine)

class Scene;

class PoolingManager : public Base
{
private:
	PoolingManager(Scene* owner) :owner(owner) {}
	virtual ~PoolingManager() = default;

public:
	static PoolingManager* Create(Scene* owner) { return new PoolingManager(owner); }

public:
	template<typename T, typename... Args>
	T* Acquire(Args&&... args){ return GetPool<T>()->Acquire(std::forward<Args>(args)...); }

	template<typename T>
	void Release(T* obj){ GetPool<T>()->Release(obj);}

	template<typename T>
	void Reserve(size_t n) { GetPool<T>()->Reserve(n); }

private:
	template<typename T>
	ObjectPool<T>* GetPool()
	{
		auto key = type_index(typeid(T));
		auto it = pools.find(key);
		if (it == pools.end())
			it = pools.emplace(key, make_unique<ObjectPool<T>>()).first;
		return static_cast<ObjectPool<T>*>(it->second.get());
	}

private:
	void Free() override {}

private:
	Scene* owner = nullptr;
	unordered_map<type_index, unique_ptr<IPoolBase>> pools;
};

END