#pragma once

//window
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

//DirectX
#include <d3d9.h>
#include <d3dx9.h>

//c, c++
#include <new>
#include <cstddef>
#include <ctime>
#include <typeindex>
#include <fstream>
#include <tchar.h>

//stl
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>
#include <string>
#include <unordered_map>
#include <memory>
#include <optional>
#include <random>
#include <cassert>

//Header
#include "Base.h"

#include "Engine_Enum.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_UIStruct.h"
#include "Engine_Typedef.h"
#include "Engine_Function.h"


//Library
#include <fmod.hpp>

#ifdef _DEBUG
//#define USE_IMGUI
#endif

#ifndef USE_IMGUI
#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif // _DEBUG
#endif	//DBG_NEW
#endif // !USE_IMGUI



#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>

#pragma warning(disable : 4251)

using namespace std;
using namespace Engine;