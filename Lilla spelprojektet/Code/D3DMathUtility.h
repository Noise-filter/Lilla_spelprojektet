#ifndef D3DMATHUTILITY_H
#define D3DMATHUTILITY_H
#include <D3DX10math.h>

#define Vec2 D3DXVECTOR2
#define Vec3 D3DXVECTOR3
#define Vec4 D3DXVECTOR4
#define Matrix D3DXMATRIX

#define PI (3.14159265358979323846f)

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
		mWorld		= Matrix(	0.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 1.0f);
				
		iTextureID	= 0;
	};

	OBJECT_WORLD_AND_TEXTURE(Matrix world, int ID)
	{
		mWorld		= world;
		iTextureID	= ID;
	};
};
enum ENTITY_FLAGS
{
	MAINBUILDING,
	SUPPLY,
	TOWER,
	ENEMY,
	GUI,
};

enum TEXTURE_FLAGS
{
	MAINBUILDINGTEXTURE1,
	SUPPLYTEXTURE1,
	TOWERTEXTURE1,
	ENEMYTEXTURE1,
};

enum LIGHT_FLAGS
{
	DIRECTIONAL,
	POINT,
	SPOT,
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