#include "Input.h"

Input::Input()
{
	this->prevMousePos = D3DXVECTOR3(0,0,0);
	this->mState = NULL;
}

bool Input::checkKeyDown(int vkCode)
{
	if(vkCode == mState->btnState)
	{
		return true;
	}

	return false;
}
bool Input::checkMovement(char input) const
{
	if(GetAsyncKeyState(input))
	{
		return true;
	}

	return false;
}

void Input::updateMs(MouseState* mState)
{
	this->mState = mState;
}

float Input::mousePitch()
{
	float dy = 0;
	if((mState->btnState & MK_LBUTTON) == 0 )
	{
		dy = D3DXToRadian(0.25f * static_cast<float>(mState->yPos - this->prevMousePos.y));
	}

	this->prevMousePos.y = mState->yPos;

	return dy* 20;
}

MouseState* Input::getMs()
{
	return this->mState;

}

void Input::resetBtnState()
{
	if(mState->btnState != 0)
		mState->btnState = 0;
		
}

float Input::mouseRotateY()
{
	float dx = 0; 
	if((mState->btnState & MK_LBUTTON) == 0)
	{
		dx = D3DXToRadian(0.25f * static_cast<float>(mState->xPos - this->prevMousePos.x));
	}

	this->prevMousePos.x = mState->xPos;

	return dx * 20;
}
