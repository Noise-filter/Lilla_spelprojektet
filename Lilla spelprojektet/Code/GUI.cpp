#include "GUI.h"

GUI::GUI()
{
	



}

GUI::~GUI()
{

}

void GUI::init()
{

	
}

void GUI::render()
{

}

int GUI::update(float dt)
{
	return 1;
}

void GUI::createBtns(GAMESTATES state)
{
	float midScreenW = (float)SCREEN_WIDTH/2;
	float midScreenH = (float)SCREEN_HEIGHT/2;

	if(this->menuBtns != NULL)
	{
		clearBtns();
	}

	if(state == STATE_MENU)
	{
		this->nrOfBtns = 3;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR3(midScreenW, 0, midScreenH), STARTGAME);
		this->menuBtns[1] = createBtn(D3DXVECTOR3(midScreenW, 0, midScreenH + 0.15), SETTINGS);
		this->menuBtns[2] = createBtn(D3DXVECTOR3(midScreenW, 0, midScreenH + 0.3), QUIT);
	}
	else if(state == STATE_SETTINGS)
	{

	}
	else if(state == STATE_PAUSED)
	{
		this->nrOfBtns = 3;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR3(midScreenW, 0, midScreenH), PAUSED_CONTINUE);
		this->menuBtns[1] = createBtn(D3DXVECTOR3(midScreenW, 0, midScreenH + 0.15), SETTINGS);
		this->menuBtns[2] = createBtn(D3DXVECTOR3(midScreenW, 0, midScreenH + 0.3), QUIT);
	}

}

void GUI::clearBtns()
{
	delete this->menuBtns;
	this->menuBtns = NULL;
}

Button GUI::createBtn(D3DXVECTOR3 pos, BUTTONTYPE type)
{
	Button temp;
	
	temp.pos = pos;
	temp.type = type;

	if(type == STARTGAME)
	{
		temp.size = D3DXVECTOR3(5, 0, 20);
		temp.text = "New Game";
	}
	if(type == SETTINGS)
	{
		temp.size = D3DXVECTOR3(5, 0, 20);
		temp.text = "Settings";
	}
	if(type == QUIT)
	{
		temp.size = D3DXVECTOR3(5, 0, 10);
		temp.text = "Quit";
	}
	if(type == PAUSED_CONTINUE)
	{
		temp.size = D3DXVECTOR3(5, 0, 20);
		temp.text = "CONTINUE";
	}
	return temp;
}

bool GUI::checkBtn(D3DXVECTOR3 pos, Button btn)
{
	return true;
}

D3DXVECTOR3 GUI::getMouseWorldPos(MouseState *mState, D3DXMATRIX view, D3DXMATRIX proj, D3DXVECTOR3 cameraPos)
{
	float pointX, pointY, intersect;
	D3DXMATRIX invView, worldIdentity;
	D3DXVECTOR3 dir, origin, rayOrigin, rayDir, intersectPos, planeNormal;
	D3DXMatrixIdentity(&worldIdentity);

	
	origin = cameraPos;
	pointX = ((2.0f * (float)mState->xPos) / (float)SCREEN_WIDTH) - 1.0f;
	pointY = (((2.0f * (float)mState->yPos) / (float)SCREEN_HEIGHT) - 1.0f) * -1.0f;

	pointX = pointX / proj._11;
	pointY = pointY / proj._22;

	D3DXMatrixInverse(&invView, NULL, &view);

	dir.x = (pointX * invView._11) + (pointY * invView._21) + invView._31;
	dir.y = (pointX * invView._12) + (pointY * invView._22) + invView._32;
	dir.z = (pointX * invView._13) + (pointY * invView._23) + invView._33;

	D3DXVec3TransformCoord(&rayOrigin, &origin, &worldIdentity);
	D3DXVec3TransformNormal(&rayDir, &dir, &worldIdentity);

	D3DXVec3Normalize(&rayDir, &rayDir);
	planeNormal = D3DXVECTOR3(0,1,0);
	intersect = (D3DXVec3Dot(&-planeNormal, &rayOrigin))/(D3DXVec3Dot(&planeNormal,&rayDir));

	return intersectPos = rayOrigin + (intersect*rayDir);
}