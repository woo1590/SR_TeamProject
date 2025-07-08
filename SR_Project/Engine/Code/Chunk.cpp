#include "EnginePCH.h"
#include "Chunk.h"
#include "ObjectManager.h"
#include "ChunkMesh.h"

//component
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "StaticBlock.h"
#include "DynamicBlock.h"

Chunk::Chunk(ObjectManager* owner, ObjectType objType, int ChunkX, int ChunkZ)
	:Object(owner,objType)
{
}

Chunk::~Chunk()
{
}

Chunk* Chunk::Create(ObjectManager* owner, ObjectType objType, int ChunkX, int ChunkZ)
{
	Chunk* Instance = new Chunk(owner, objType, ChunkX, ChunkZ);

	if (FAILED(Instance->Ready_Object()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT Chunk::Ready_Object()
{
	Blocks.clear();
	Blocks.resize(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);

	mesh = ChunkMesh::Create();

	auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_NonAlpha);
	renderer->SetMesh(mesh);
	renderer->SetMaterial("Chunk_Mtrl");

	auto transform = AddComponent<TransformComponent>();

	return S_OK;
}

void Chunk::Update(_float dt)
{
	if (Is_Dirty)
		BuildMesh();

	Object::Update(dt);
	Object::Late_Update(dt);
}

void Chunk::AddBlock(int lx, int ly, int lz, SB sb)
{
	int idx = Index(lx, ly, lz);

	Blocks[idx] = sb;

	Is_Dirty = true;
}

void Chunk::BuildMesh()
{
	// 6방향 오프셋 (X+,X-,Y+,Y-,Z+,Z-)
	static const int dx[6] = { +1, -1,  0,  0,  0,  0 };
	static const int dy[6] = { 0,  0, +1, -1,  0,  0 };
	static const int dz[6] = { 0,  0,  0,  0, +1, -1 };

	// 한 면(face)마다 정점 4개 상대 오프셋 (pos + offsets)
	static const _vec3 faceOffsets[6][4] = {
		// X+ face
		{{1,0,0}, {1,0,1}, {1,1,1}, {1,1,0}},
		// X- face
		{{0,0,1}, {0,0,0}, {0,1,0}, {0,1,1}},
		// Y+ face
		{{0,1,0}, {1,1,0}, {1,1,1}, {0,1,1}},
		// Y- face
		{{0,0,1}, {1,0,1}, {1,0,0}, {0,0,0}},
		// Z+ face
		{{0,0,1}, {0,1,1}, {1,1,1}, {1,0,1}},
		// Z- face
		{{1,0,0}, {1,1,0}, {0,1,0}, {0,0,0}}
	};

	static const _vec2 uv[4] =
	{
		{0.f,1.f},
		{0.f,0.f},
		{1.f,0.f},
		{1.f,1.f}
	};

	std::vector<VTXTEX> verts;
	std::vector<INDEX32> inds;

	auto isAir = [&](int x, int y, int z)
		{
			if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE)
				return true;
			return Blocks[Index(x, y, z)].Type == Air;
		};

	for (int z = 0; z < CHUNK_SIZE; ++z)
		for (int y = 0; y < CHUNK_SIZE; ++y)
			for (int x = 0; x < CHUNK_SIZE; ++x)
			{
				if (Blocks[Index(x, y, z)].Type == Air)
					continue;

				_vec3 worldPos{ (float)ChunkX * CHUNK_SIZE + x,(float)y,(float)ChunkZ * CHUNK_SIZE + z };

				for (int dir = 0; dir < 6;++dir)
				{
					int nx = x + dx[dir];
					int ny = y + dy[dir];
					int nz = z + dz[dir];

					if (!isAir(nx, ny, nz)) continue;

					_ulong start = verts.size();
					for (int i = 0; i < 4; ++i)
					{
						VTXTEX v;
						v.vPosition = worldPos + faceOffsets[dir][i];
						v.vNormal = _vec3(dx[dir], dy[dir], dz[dir]);
						v.vTexUV = uv[i];

						verts.push_back(v);
					}

					inds.push_back({ start,start + 1,start + 2 });
					inds.push_back({ start,start + 2,start + 3 });
				}

			}

	mesh->SetBuffer(verts, inds);
	Is_Dirty = false;
}

int Chunk::Index(int lx, int ly, int lz)
{
	return lx
		+ ly * CHUNK_SIZE
		+ lz * (CHUNK_SIZE * CHUNK_SIZE);
}

void Chunk::Free()
{
}
