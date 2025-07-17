#ifndef Engine_Macro_h__
#define Engine_Macro_h__

namespace Engine
{
	constexpr float WINCX = 1280;
	constexpr float WINCY = 720;

	constexpr int CHUNK_SIZE = 16;
	constexpr int CHUNK_HEIGHT = 96;
	constexpr float BLOCK_SIZE = 2.f;

#ifndef MSG_BOX
#define MSG_BOX(_message)		MessageBox(NULL, TEXT(_message), L"System Message", MB_OK);
#endif

#define		BEGIN(NAMESPACE)	namespace NAMESPACE{
#define		END					}

#define		USING(NAMESPACE)	using namespace NAMESPACE;

#ifdef ENGINE_EXPORTS
#define ENGINE_DLL		_declspec(dllexport)
#else
#define	ENGINE_DLL		_declspec(dllimport)
#endif

#define	NO_COPY(CLASSNAME)								\
		private:										\
		CLASSNAME(const CLASSNAME&) = delete;			\
		CLASSNAME& operator = (const CLASSNAME&) = delete;

#define	DECLARE_SINGLETON(CLASSNAME)					\
		NO_COPY(CLASSNAME)								\
		private:										\
		static CLASSNAME* m_pInstance;					\
		public:											\
		static CLASSNAME* GetInstance( void );			\
		static void DestroyInstance( void );			\

#define	IMPLEMENT_SINGLETON(CLASSNAME)					\
		CLASSNAME*	CLASSNAME::m_pInstance = nullptr;	\
		CLASSNAME*	CLASSNAME::GetInstance( void ){		\
			if(!m_pInstance){							\
				m_pInstance = new CLASSNAME;			\
			}											\
			return m_pInstance;							\
		}												\
		void CLASSNAME::DestroyInstance( void ){		\
			if (m_pInstance){							\
				delete m_pInstance;						\
				m_pInstance = nullptr;					\
			}											\
		}

#define LOG(msg) OutputDebugStringW((std::wstring(msg) + L"\n").c_str())

	// 포맷 문자열 지원용
#define LOGF(fmt, ...)                     \
{                                          \
    wchar_t buf[512];                      \
    swprintf_s(buf, 512, fmt, __VA_ARGS__);\
    OutputDebugStringW(buf);              \
    OutputDebugStringW(L"\n");             \
}
}

#endif // Engine_Macro_h__
