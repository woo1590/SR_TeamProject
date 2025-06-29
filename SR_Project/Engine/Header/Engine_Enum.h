#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum class WINMODE { Mode_Full, Mode_Win };

	enum class COMPONENTID { ID_Dynamic, ID_Static, ID_End };

	enum class INFO { Info_Right, Info_Up, Info_Look, Info_Pos, Info_End };
	enum class ROTATION { Rot_X, Rot_Y, Rot_Z, Rot_End };

	enum class TEXTURE { Tex_Diffuse, Tex_Cube, Tex_Normal ,Tex_End };

	enum class RENDER_ID { Render_Priority, Render_NonAlpha, Render_Mask, Render_Shadow, Render_Reflect, Render_Alpha, Render_UI, Render_End };

	enum class MOUSEKEYSTATE { Dim_Lb, Dim_Rb, Dim_Mb, Dim_End };
	enum class MOUSEMOVESTATE { Dims_X, Dims_Y, Dims_Z, Dims_End };


	/*----------------------my enum----------------------*/
	enum class ObjectType
	{
		Player,
		Monster,
		Light,
		Terrain,
		SkyBox,
		Camera,
		UI,
		Block,
		Count
	};

	enum BlockType { Dirt, GrassDirt };
}
#endif // Engine_Enum_h__
