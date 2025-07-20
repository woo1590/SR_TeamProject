#include "EnginePCH.h"
#include "ObjectManager.h"
#include "Object.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "UIRenderer.h"

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
	auto uiState = UIRenderer::GetCurRenderType();
	for (int type = 0; type < static_cast<int>(ObjectType::Count); ++type)
	{
		if (type == static_cast<int>(ObjectType::CollisionBlock))
			continue;

		for (auto obj : Objects[type])
		{
			if (type == static_cast<int>(ObjectType::UI))
			{
				auto ui = obj->GetComponent<UIRenderer>();
				if (ui->GetRenderType() != uiState && ui->GetRenderType() != UIRenderType::Always)
					continue;
			}
			obj->Update(dt);
		}
	}
}

void ObjectManager::Late_Update(float dt)
{
	auto uiState = UIRenderer::GetCurRenderType();
	for (int type = 0; type < static_cast<int>(ObjectType::Count); ++type)
	{
		if (type == static_cast<int>(ObjectType::CollisionBlock)) continue;

		auto& list = Objects[type];
		for (auto it = list.begin(); it != list.end();)
		{
			auto obj = *it;
			if (obj->IsDead())
			{
				obj->UnRegister();
				Safe_Release(obj);
				it = list.erase(it);
			}
			else
			{
				if (type == static_cast<int>(ObjectType::UI))
				{
					auto ui = obj->GetComponent<UIRenderer>();
					if (ui->GetRenderType() != uiState && ui->GetRenderType() != UIRenderType::Always)
					{
						++it;
						continue;
					}
				}
				obj->Late_Update(dt);
				++it;
			}
		}
	}
}

void ObjectManager::AddObject(ObjectType objType, Object* object)
{
	Objects[static_cast<int>(objType)].push_back(object);
}

void ObjectManager::RemoveObject(ObjectType objType, const _vec3& worldPosition)
{
	auto& list = Objects[(int)objType];
	auto it = std::find_if(list.begin(), list.end(),
		[&](Object* obj)
		{
			auto tf = obj->GetComponent<TransformComponent>();
			return tf && tf->GetPosition() == worldPosition;
		});

	if (it != list.end())
	{
		Safe_Release(*it);
		list.erase(it);
	}
}

void ObjectManager::ClearList(ObjectType objType)
{
	for (auto& object : Objects[static_cast<int>(objType)])
		Safe_Release(object);
	Objects[static_cast<int>(objType)].clear();
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

Scene* ObjectManager::GetOwner() const
{
	return owner;
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
