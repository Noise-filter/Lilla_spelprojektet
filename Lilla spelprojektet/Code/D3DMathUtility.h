#ifndef D3DMATHUTILITY_H
#define D3DMATHUTILITY_H
#include <D3DX10math.h>

#define Vec2 D3DXVECTOR2
#define Vec3 D3DXVECTOR3
#define Vec4 D3DXVECTOR4
#define Matrix D3DXMATRIX

#define PI (3.14159265358979323846f)


struct MESH_P
{
	Vec3 pos;
	float pad;
	MESH_P()
	{
		this->pos = Vec3(0.0f, 0.0f, 0.0f);
		pad = 0;
	};

	MESH_P(Vec3 pos)
	{
		this->pos = pos;
		pad = 9;
	};
};

struct MESH_PUV
{
	Vec3 pos;
	Vec2 uv;

	MESH_PUV()
	{
		this->pos = Vec3(0.0f, 0.0f, 0.0f);
		this->uv	  = Vec2(0.0f, 0.0f);
	};

	MESH_PUV(Vec3 pos, Vec2 uv)
	{
		this->pos	= pos;
		this->uv		= uv;
	};
};

struct MESH_PNUV
{
	Vec3 pos;
	Vec3 normal;
	Vec2 uv;

	MESH_PNUV()
	{
		this->pos		= Vec3(0.0f, 0.0f, 0.0f);
		this->normal		= Vec3(0.0f, 0.0f, 0.0f);
		this->uv			= Vec2(0.0f, 0.0f);
	};
	
	MESH_PNUV(Vec3 pos, Vec3 normal, Vec2 uv)
	{
		this->pos		= pos;
		this->normal		= normal;
		this->uv			= uv;
	};
};


struct OBJECT_WORLD_AND_TEXTURE
{
	Matrix mWorld;
	int iTextureID;

	OBJECT_WORLD_AND_TEXTURE()
	{
		iTextureID	= 0;
		mWorld		= Matrix(0.0f, 0.0f, 0.0f, 0.0f,
							 0.0f, 0.0f, 0.0f, 0.0f,
							 0.0f, 0.0f, 0.0f, 0.0f,
							 0.0f, 0.0f, 0.0f, 1.0f);
	};

	OBJECT_WORLD_AND_TEXTURE(Matrix world, int ID)
	{
		iTextureID	= ID;
		mWorld		= world;
	};
};
enum ENTITY_FLAGS
{
	ENTITY_MAINBUILDING,
	ENTITY_SUPPLY,
	ENTITY_TOWER,
	ENTITY_ENEMY,
	ENTITY_GUI,
	ENTITY_PARTICLESYSTEM,
	ENTITY_GAMEFIELD,
};

enum TEXTURE_FLAGS
{
	TEXTURE_MAINBUILDING1,
	TEXTURE_SUPPLY1,
	TEXTURE_TOWER1,
	TEXTURE_ENEMY1,
	TEXTURE_GUI1,
	TEXTURE_PARTICLESYSTEM1,
	TEXTURE_GAMEFIELD1,
};

enum LIGHT_FLAGS
{
	LIGHT_NONE			= -1,
	LIGHT_DIRECTIONAL	= 0,
	LIGHT_POINT			= 1,
	LIGHT_SPOT			= 2,
};

struct RENDERDATA
{
	int iEntityID;
	OBJECT_WORLD_AND_TEXTURE worldTex;
	int iLightID;

	RENDERDATA()
	{
		iEntityID	= 0;
		worldTex	= OBJECT_WORLD_AND_TEXTURE();
		iLightID	= 0;
	};

	RENDERDATA(int entityID, OBJECT_WORLD_AND_TEXTURE wTex, int lightID)
	{
		iEntityID	= entityID;
		worldTex	= wTex;
		iLightID	= lightID;
	};
};



#endif