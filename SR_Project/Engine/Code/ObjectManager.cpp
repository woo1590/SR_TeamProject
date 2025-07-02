#include "EnginePCH.h"
#include "ObjectManager.h"
#include "Object.h"

ObjectManager::ObjectManager(Scene* owner)
	:owner(owner)
{
}

ObjectManager::~ObjectManager()
{
}

ObjectManager* ObjectManager::Create(Scene* owner)
{
	ObjectManager* Instance = new ObjectManager(owner);
	if (FAILED(Instance->Ready_ObjectMgr()))
	{
		Safe_Release(Instance);

		Instance = nullptr;
	}

	return Instance;
}

HRESULT ObjectManager::Ready_ObjectMgr()
{
	Objects.resize(static_cast<int>(ObjectType::Count));
	return S_OK;
}

void ObjectManager::Update(float dt)
{
	for (int type = 0; type < static_cast<int>(ObjectType::Count); ++type)
	{
		for (const auto& object : Objects[type])
		{
			object->Update(dt);
		}
	}
}

void ObjectManager::Late_Update(float dt)
{
	for (int type = 0; type < static_cast<int>(ObjectType::Count); ++type)
	{
 		for (const auto& object : Objects[type])
		{
			object->Late_Update(dt);
		}
	}
}

void ObjectManager::AddObject(ObjectType objType, Object* object)
{
	Objects[static_cast<int>(objType)].push_back(object);
}

void ObjectManager::AddUIObject(Object* obj)
{
	Objects[static_cast<int>(ObjectType::UI)].push_back(obj);
}

Object* ObjectManager::GetFrontObject(ObjectType objType)
{
	return Objects[static_cast<int>(objType)].front();
}

std::list<Object*> ObjectManager::GetObjectList(ObjectType objType)
{
	return Objects[static_cast<int>(objType)];
}

void ObjectManager::Free()
{
	for (int type = 0; type < static_cast<int>(ObjectType::Count); ++type)
	{
		for (auto& object : Objects[type])
		{
			Safe_Release(object);
		}
		Objects[type].clear();
	}
	Objects.clear();
}
