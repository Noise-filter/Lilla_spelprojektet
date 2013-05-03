#pragma once
#include "D3DGraphicUtility.h"
#include "D3DMathUtility.h"
#include "Buffer.h"
#include "Shader.h"
#include <string>

using namespace std;

enum GAMESTATES
{
   STATE_MENU,
   STATE_SETTINGS,
   STATE_GAMESTART,
   STATE_PLAYING,
   STATE_PAUSED, 
   STATE_WIN,
   STATE_LOSE
};

enum BUTTONTYPE
{
	STARTGAME,
	SETTINGS,
	QUIT,
	PAUSED_CONTINUE
};

struct Button
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 size;
		string text;
		BUTTONTYPE type;

		
	};

class GUI
{
public:
	GUI(void);
	~GUI(void);

	void init();
	void render();
	int update(float dt);
	void createBtns(GAMESTATES state);
	void clearBtns();
	Button createBtn(D3DXVECTOR3 pos, BUTTONTYPE type);

private:
	
	
	D3DXVECTOR3 getMouseWorldPos(MouseState *mState, D3DXMATRIX view, D3DXMATRIX proj, D3DXVECTOR3 cameraPos);
	bool checkBtn(D3DXVECTOR3 mousePos, Button btn);
	Button* menuBtns;
	int nrOfBtns;





};