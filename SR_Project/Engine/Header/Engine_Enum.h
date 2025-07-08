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
		StaticBlock,
		DynamicBlock,
		Part,
		Count
	};

	enum class BoundingBoxType
	{
		Sphere,
		Box,
		Count
	};

	enum class NotifyType
	{
		None, 
		HP_Changed,
		EXP_Changed,
		Speed_Changed,
		Player_Damaged
	};

	enum class BarDirection {Vertical, Horizontal, ClockWise16};

	enum class UIPivot {Center, Bottom, LeftTop, Left, Right};

	enum class SlotSize {Small, Default, Large};

	enum class Color
	{
		White,
		Black,
		Red,
		Green,
		Blue,
		Yellow,
		Cyan,
		Magenta,
		Pink,
		Orange,
		Gray,
		SkyBlue,
		Purple,
		Brown,
		Transparent
	};

	enum class FontType
	{
		Regular,     // �⺻: 18pt, FW_NORMAL
		Bold,        // ����: 18pt, FW_BOLD
		Small,       // �۰�: 12pt, FW_NORMAL
		Large,       // ũ��: 28pt, FW_NORMAL
		Title,       // Ÿ��Ʋ��: 36pt, FW_BOLD
		Custom       // ����� ���� (������ Ȯ���)
	};

	inline D3DXCOLOR ToD3DXColor(Color color)
	{
		switch (color)
		{
		case Color::White:      return D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		case Color::Black:      return D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
		case Color::Red:        return D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
		case Color::Green:      return D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
		case Color::Blue:       return D3DXCOLOR(0.f, 0.f, 1.f, 1.f);
		case Color::Yellow:     return D3DXCOLOR(1.f, 1.f, 0.f, 1.f);
		case Color::Cyan:       return D3DXCOLOR(0.f, 1.f, 1.f, 1.f);
		case Color::Magenta:    return D3DXCOLOR(1.f, 0.f, 1.f, 1.f);
		case Color::Pink:       return D3DXCOLOR(1.f, 0.75f, 0.8f, 1.f);
		case Color::Orange:     return D3DXCOLOR(1.f, 0.5f, 0.f, 1.f);
		case Color::Gray:       return D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f);
		case Color::SkyBlue:    return D3DXCOLOR(0.53f, 0.81f, 0.92f, 1.f);
		case Color::Purple:     return D3DXCOLOR(0.5f, 0.f, 0.5f, 1.f);
		case Color::Brown:      return D3DXCOLOR(0.6f, 0.4f, 0.2f, 1.f);
		case Color::Transparent:return D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		default:                return D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
		}
	}

	enum StaticBlockAxis { sAX, sAY, sAZ, sAEnd };
	enum StaticBlockRot { sZP, sZM, sXP, sXM, sREnd };
	enum StaticBlockType
	{
		sBlockNone,
		Dirt, GrassDirt, Wood, WoodPlank,
		Stone, CobbleStone, SmoothStone, StoneBrick, MossyStoneBrick,
		Glass, Leaf, Air,
		sBlockEnd
	};
	enum StaticBlockUsage { Basic, Half, Stair, Alpha, Fence };

	enum DynamicBlockCol { dZP, dZM, dXP, dXM, dYP, dAEnd };
	enum DynamicBlockRot { drXP, drXM, drZP, drZM, dREnd };
	enum DynamicBlockType
	{
		dBlockNone,
		LeverSwitch, BasicChest, IronCages, WoodBridge,
		dBlockEnd
	};

	enum FaceDir { Face_Top, Face_Bottom, Face_Left, Face_Right, Face_Front, Face_Behind };
}
#endif // Engine_Enum_h__
