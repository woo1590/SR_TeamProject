#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum WINMODE { MODE_FULL, MODE_WIN };

	enum COMPONENTID { ID_DYNAMIC, ID_STATIC, ID_END };

	enum INFO { INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };
	enum ROTATION { ROT_X, ROT_Y, ROT_Z, ROT_END };

	enum TEXTURE { TEX_NORMAL, TEX_CUBE, TEX_END };

	enum RENDER_ID { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_MASK, RENDER_SHADOW, RENDER_REFLECT, RENDER_ALPHA, RENDER_UI, RENDER_END };

	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	/*----------------------my enum----------------------*/
	enum class ObjectType
	{
		Player,
		Light,
		Terrain,
		SkyBox,
		Camera,
		Count
	};
}
#endif // Engine_Enum_h__
