#include "Camera.h"

Camera::Camera()
{
	this->m_position = Vec3(0, 50, 0);
	this->mRight = Vec3(1, 0, 0);
	this->mUp = Vec3(0, 1, 0);
	this->mLook = Vec3(0, 0, 1);
}

Camera::~Camera()
{
}

void Camera::SetPosition(const Vec3& v)
{
	this->m_position = v;
}

Vec3 Camera::GetPosition()const
{
	return this->m_position;
}

Vec3 Camera::GetRight()const
{
	return this->mRight;
}

Vec3 Camera::GetUp()const
{
	return this->mUp;
}

Vec3 Camera::GetLook()const
{
	return this->mLook;
}

float Camera::GetNearZ()const
{
	return this->mNearZ;
}

float Camera::GetFarZ()const
{
	return this->mFarZ;
}

float Camera::GetAspect()const
{
	return this->mAspect;
}

Vec3 Camera::CrossMatrix(const Vec3& vector, const D3DXMATRIX& matrix)
{
	Vec3 vec;
	vec.x = matrix._11*vector.x + matrix._12*vector.y + matrix._13*vector.z;
	vec.y = matrix._21*vector.x + matrix._22*vector.y + matrix._23*vector.z;
	vec.z = matrix._31*vector.x + matrix._32*vector.y + matrix._33*vector.z;
	return vec;
}

void Camera::SetLens(float fovY, float aspect, float zn, float zf)
{
	this->mFovY = fovY;
	this->mAspect = aspect;
	this->mNearZ = zn;
	this->mFarZ = zf;

	float yScale = tan((PI*0.5f) - (mFovY*0.5f));
	float xScale = yScale/this->mAspect;

	mProj = D3DXMATRIX(xScale, 0, 0, 0, 
					   0, yScale, 0, 0,
					   0, 0, zf/(zf-zn), 1,
					   0, 0, -zn*zf/(zf-zn), 0);
}

void Camera::LookAt(Vec3 pos, Vec3 target, Vec3 worldUp)
{
	Vec3 L;
	D3DXVec3Subtract(&L, &target, &pos);
	D3DXVec3Normalize(&L, &L);
	Vec3 R; 
	D3DXVec3Cross(&R, &worldUp, &L);
	D3DXVec3Normalize(&R, &R);
	Vec3 U;
	D3DXVec3Cross(&U, &L, &R);

	this->m_position = pos;
	this->mLook = L;
	this->mRight = R;
	this->mUp = U;
}

D3DXMATRIX Camera::View()const
{
	return this->mView;
}

D3DXMATRIX Camera::Proj()const
{
	return this->mProj;
}

D3DXMATRIX Camera::ViewsProj()const
{
	D3DXMATRIX M;
	D3DXMatrixMultiply(&M, &this->mView, &this->mProj);
	return M;
}

void Camera::Walk(float dist)
{
	this->m_position += dist*Vec3(1,0,0);
}

void Camera::Strafe(float dist)
{
	this->m_position += dist*this->mRight;
}

void Camera::Pitch(float angle)
{
	float radians = angle * 0.0174532925f;
	
	D3DXMATRIX R;
							  
	D3DXMatrixRotationAxis(&R, &-mRight, radians);
	this->mUp = CrossMatrix(this->mUp, R);
	this->mLook = CrossMatrix(this->mLook, R);
}

void Camera::Yaw(float angle)
{
	float radians = angle * 0.0174532925f;
	
	D3DXMATRIX R;
							  
	Vec3 up(0,1,0);
	D3DXMatrixRotationAxis(&R, &-up, radians);

	this->mRight = CrossMatrix(this->mRight, R);
	this->mUp = CrossMatrix(mUp, R);
	this->mLook = CrossMatrix(this->mLook, R);
}

void Camera::UpdateViewMatrix()
{
	D3DXVec3Normalize(&this->mLook, &this->mLook);
	D3DXVec3Cross(&this->mUp, &this->mLook, &this->mRight);
	D3DXVec3Normalize(&this->mUp, &this->mUp);
	D3DXVec3Cross(&this->mRight, &this->mUp, &this->mLook);
	

	float x = -D3DXVec3Dot(&this->m_position, &mRight);
	float y = -D3DXVec3Dot(&this->m_position, &mUp);
	float z = -D3DXVec3Dot(&this->m_position, &mLook);

	this->mView(0, 0) = this->mRight.x;
	this->mView(1, 0) = this->mRight.y;
	this->mView(2, 0) = this->mRight.z;
	this->mView(3, 0) = x;

	this->mView(0, 1) = this->mUp.x;
	this->mView(1, 1) = this->mUp.y;
	this->mView(2, 1) = this->mUp.z;
	this->mView(3, 1) = y;

	this->mView(0, 2) = this->mLook.x;
	this->mView(1, 2) = this->mLook.y;
	this->mView(2, 2) = this->mLook.z;
	this->mView(3, 2) = z;

	this->mView(0, 3) = 0.0f;
	this->mView(1, 3) = 0.0f;
	this->mView(2, 3) = 0.0f;
	this->mView(3, 3) = 1.0f;
}