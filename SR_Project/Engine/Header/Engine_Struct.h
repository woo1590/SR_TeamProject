#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"

namespace Engine
{
	typedef struct tagVertexColor
	{
		_vec3	vPosition;
		_ulong	dwColor;

	}VTXCOL;

	constexpr _ulong	FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;	//tex0은 없어도 됨, 텍스쳐가 없다는 의미를 나타내는 관습

	typedef struct tagVertexTexture
	{
		_vec3	vPosition;
		_vec2	vTexUV;

	}VTXTEX;

	constexpr _ulong	FVF_TEX = D3DFVF_XYZ | D3DFVF_TEX1;

	typedef struct tagVertexCubeTexture
	{
		_vec3	vPosition;
		_vec3	vTexUV;

	}VTXCUBE;

	constexpr _ulong	FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	typedef struct tagIndex16
	{
		_ushort	_0;
		_ushort _1;
		_ushort _2;

	}INDEX16;

	typedef struct tagIndex32
	{
		_ulong	_0;
		_ulong	_1;
		_ulong	_2;
	}INDEX32;

	/*--------------------My Struct-------------*/
	typedef struct tagVertexNormal
	{
		_vec3 vPosition;
		_vec3 vNormal{ 0.f,0.f,0.f };
		_vec2 vTexUV{ 0.f,0.f };
	}VTXNOR;

	constexpr _ulong FVF_NOR = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct BlockData
	{
		_vec3 Pos;
		BlockType Type;
	}BLOCK;
}

#endif // Engine_Struct_h__
