#ifndef Engine_Function_h__
#define Engine_Function_h__

#include "Engine_Typedef.h"

namespace Engine
{
	// 템플릿은 기능의 정해져있으나 자료형은 정해져있지 않은 것
	// 기능을 인스턴스화 하기 위하여 만들어두는 틀

	template<typename T>
	void Safe_Delete(T& p)
	{
		if (p)
		{
			delete p;
			p = nullptr;
		}
	}

	template<typename T>
	void Safe_Delete_Array(T& p)
	{
		if (p)
		{
			delete[] p;
			p = nullptr;
		}
	}

	template<typename T>
	_ulong Safe_Release(T& instance)
	{
		_ulong dwRefCnt = 0;

		if (instance)
		{
			dwRefCnt = instance->Release();

			if (dwRefCnt)
				instance = nullptr;
		}

		return dwRefCnt;
	}

	//Functor
	class Tag_Finder
	{
	public:
		explicit Tag_Finder(const _tchar* tag) :m_pTargetTag(tag) {}
		~Tag_Finder(void) {}
		
	public:
		template<typename T>
		_bool		operator()(const T& pair)
		{
			if (lstrcmpW(m_pTargetTag, pair.first))
				return true;

			return false;
		}

	private:
		const _tchar* m_pTargetTag = nullptr;
	};

	class DeleteObject
	{
	public:
		explicit DeleteObject(void){}
		~DeleteObject(){}

	public:
		template<typename T>
		void operator ()(T& instance)
		{
			_ulong dwRefCnt = 0;

			dwRefCnt = instance->Release();

			if (!dwRefCnt)
				instance = nullptr;
		}
	};

	class DeleteMap
	{
	public:
		explicit DeleteMap(void){}
		~DeleteMap(){}

	public:
		template<typename T>
		void operator()(T& pair)
		{
			_ulong dwRefCnt = 0;

			dwRefCnt = pair.second->Release();

			if (!dwRefCnt)
				pair.second = nullptr;
		}
	};
}

#endif // Engine_Function_h__
