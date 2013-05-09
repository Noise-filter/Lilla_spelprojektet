#include "GUI.h"

GUI::GUI()
{
	this->menuBtns = NULL;
	this->textObjects = NULL;
	createBtns(STATE_MENU);


}

GUI::~GUI()
{
	SAFE_DELETE_ARRAY(textObjects);
	SAFE_DELETE(menuBtns);
}


void GUI::render(ID3D11Device *pDevice, ID3D11DeviceContext *pContext)
{
	for(int i = 0; i < this->nrOfBtns; i++)
	{
		this->textObjects[i].drawText(pDevice, pContext);
	}
}

int GUI::update(MouseState *mouse, GAMESTATES state)
{
	if(GUI_STATE != state)
	{
		createBtns(state);
		GUI_STATE = state;
	}

	for(int i = 0; i < this->nrOfBtns; i++)
	{
		bool check = checkBtn(mouse, this->menuBtns[i]);
		if(check)
		{
			state = changeState(this->menuBtns[i]);
		}
	}
	return 1;
}

void GUI::createBtns(GAMESTATES state)
{
	float midScreenW = (float)SCREEN_WIDTH/2;
	float midScreenH = (float)SCREEN_HEIGHT/2;
	int a=5;
	cout << a<<endl;

	if(this->menuBtns != NULL)
	{
		clearBtns();
	}

	if(state == STATE_MENU)
	{
		this->nrOfBtns = 3;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR2(midScreenW, midScreenH), STARTGAME);
		this->menuBtns[1] = createBtn(D3DXVECTOR2(midScreenW, midScreenH + 0.15f), SETTINGS);
		this->menuBtns[2] = createBtn(D3DXVECTOR2(midScreenW, midScreenH + 0.3f), QUIT);
	}
	else if(state == STATE_SETTINGS)
	{

	}
	else if(state == STATE_PAUSED)
	{
		this->nrOfBtns = 3;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR2(midScreenW, midScreenH), PAUSED_CONTINUE);
		this->menuBtns[1] = createBtn(D3DXVECTOR2(midScreenW, midScreenH + 0.15f), SETTINGS);
		this->menuBtns[2] = createBtn(D3DXVECTOR2(midScreenW, midScreenH + 0.3f), QUIT);
	}
	if(this->nrOfBtns != 0)
	{
		this->textObjects = new TextObject[nrOfBtns];
		for(int i = 0; i < this->nrOfBtns; i++)
		{
			this->textObjects[i] = TextObject(menuBtns[i].text, 20, menuBtns[i].pos, menuBtns[i].textColor);
		}
	}
}

void GUI::clearBtns()
{
	delete this->menuBtns;
	this->menuBtns = NULL;
}

Button GUI::createBtn(D3DXVECTOR2 pos, BUTTONTYPE type)
{
	Button temp;
	
	temp.pos = pos;
	temp.type = type;
	temp.textColor = 0xffffffff;

	if(type == STARTGAME)
	{
		temp.size = D3DXVECTOR2(20, 5);
		temp.text = L"New Game";
	}
	if(type == SETTINGS)
	{
		temp.size = D3DXVECTOR2(20, 5);
		temp.text = L"Settings";
	}
	if(type == QUIT)
	{
		temp.size = D3DXVECTOR2(10, 5);
		temp.text = L"Quit";
	}
	if(type == PAUSED_CONTINUE)
	{
		temp.size = D3DXVECTOR2(20, 5);
		temp.text = L"CONTINUE";
	}
	return temp;
}

bool GUI::checkBtn(MouseState *mousePos, Button btn)
{
	if(mousePos->xPos > btn.pos.x - btn.size.x)
	{
		if(mousePos->xPos < btn.pos.x + btn.size.x)
		{
			if(mousePos->yPos < btn.pos.y + btn.size.y)
			{
				if(mousePos->yPos > btn.pos.y - btn.size.y)
				{
					return true;
				}
			}
		}
	}
	return false;
}

GAMESTATES GUI::changeState(Button btn)
{
	GAMESTATES state;

	if(btn.type == STARTGAME)
	{
		state = STATE_GAMESTART;
	}
	else if(btn.type == SETTINGS)
	{
		state = STATE_SETTINGS;
	}
	else if(btn.type == PAUSED_CONTINUE)
	{
		state = STATE_PLAYING;
	}
	else if(btn.type == QUIT)
	{
		state = STATE_QUIT;
	}

	return state;
}