#pragma once

#include "ObjectPool.h"

BEGIN(Engine)

class Scene;

class PoolingManager : public Base
{
private:
	PoolingManager(Scene* owner)
		:owner(owner) {}
	virtual ~PoolingManager() = default;

public:
	static PoolingManager* Create(Scene* owner) { return new PoolingManager(owner); }

public:
	template<typename T, typename... Args>
	T* Acquire(Args&&... args)
	{
		return GetPool<T>().Acquire(foward<Args>(args)...);
	}

	template<typename T>
	void Release(T* obj)
	{
		GetPool<T>().Release(obj);
	}

	template<typename T>
	void Reserve(size_t n)
	{
		GetPool<T>().Reserve(n);
	}

private:
	template<typename T>
	static auto& GetPool()
	{
		static ObjectPool<T> pool;
		return pool;
	}


private:
	void Free() override {}


private:
	Scene* owner = nullptr;
};

END