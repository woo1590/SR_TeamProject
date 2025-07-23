#ifndef Engine_Struct_h__
#define Engine_Struct_h__

#include "Engine_Typedef.h"

namespace Engine
{
	typedef struct tagVertexPoint
	{
		_vec3 position;
		_float size;
		_vec4 color;

	}VTXPOINT;

	typedef struct tagVertexColor
	{
		_vec3	vPosition;
		_ulong	dwColor;

	}VTXCOL;

	constexpr _ulong	FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;	//tex0�� ��� ��, �ؽ��İ� ���ٴ� �ǹ̸� ��Ÿ���� ����

	typedef struct tagVertexTexture
	{
		_vec3	vPosition;
		_vec3	vNormal;
		_vec2	vTexUV;

	}VTXTEX;

	constexpr _ulong	FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagVertexCubeTexture
	{
		_vec3	vPosition;
		_vec3	vNormal;
		_vec3	vTexUV;

	}VTXCUBE;

	constexpr _ulong	FVF_CUBE = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

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

	typedef struct tagRay
	{
		_vec3 Origin;
		_vec3 Direction;
	}Ray;

	typedef struct StaticBlockData
	{
		_vec3 Pos;
		StaticBlockType Type;
		StaticBlockAxis Axis;
		StaticBlockRot Rot;
		StaticBlockUsage Usage;
	} SB;
	
	typedef struct ChunkData
	{
		const int chunkX, chunkZ;	// 청크의 좌표 위치
		std::vector<SB> sBlocks;	// 해당 청크에서 보관하는 Static Block
	} CHUNK;

	typedef struct DynamicBlockData
	{
		_vec3 Pos;
		DynamicBlockType Type;
		DynamicBlockCol Col;
		DynamicBlockRot Rot;
	} DB;

	typedef struct PrefabData
	{
		_vec3 localPos;
		StaticBlockType Type;
		StaticBlockAxis Axis;
		StaticBlockRot Rot;
		StaticBlockUsage Usage;
	} PREFAB;

	typedef struct MiniMapChunkData
	{
		bool path[CHUNK_SIZE][CHUNK_SIZE]{ false };
	} MINIMAP;
}

#endif // Engine_Struct_h__
