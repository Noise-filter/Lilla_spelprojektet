#ifndef D3DMATHUTILITY_H
#define D3DMATHUTILITY_H
#include <D3DX10math.h>

#define Vec2 D3DXVECTOR2
#define Vec3 D3DXVECTOR3
#define Vec4 D3DXVECTOR4
#define Matrix D3DXMATRIX

#define PI (3.14159265358979323846f)

struct MeshPUV
{
	Vec3 pos;
	Vec2 uv;

	MeshPUV()
	{
		this->pos = Vec3(0.0f, 0.0f, 0.0f);
		this->uv	= Vec2(0.0f, 0.0f);
	}

	MeshPUV(Vec3 pos, Vec2 uv)
	{
		this->pos	= pos;
		this->uv		= uv;
	}
};

struct MeshPNUV
{
	Vec3 pos;
	Vec3 normal;
	Vec2 uv;

	MeshPNUV()
	{
		this->pos		= Vec3(0.0f, 0.0f, 0.0f);
		this->normal		= Vec3(0.0f, 0.0f, 0.0f);
		this->uv			= Vec2(0.0f, 0.0f);
	}
	
	MeshPNUV(Vec3 pos, Vec3 normal, Vec2 uv)
	{
		this->pos		= pos;
		this->normal		= normal;
		this->uv			= uv;
	}
};

#endif