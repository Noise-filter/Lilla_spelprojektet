#include "GUI.h"

GUI::GUI()
{
	
	createBtns(STATE_MENU);


}

GUI::~GUI()
{

}


void GUI::render()
{

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

	if(this->menuBtns != NULL)
	{
		clearBtns();
	}

	if(state == STATE_MENU)
	{
		this->nrOfBtns = 3;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR3(midScreenW, midScreenH, 0), STARTGAME);
		this->menuBtns[1] = createBtn(D3DXVECTOR3(midScreenW, midScreenH + 0.15, 0), SETTINGS);
		this->menuBtns[2] = createBtn(D3DXVECTOR3(midScreenW, midScreenH + 0.3, 0), QUIT);
	}
	else if(state == STATE_SETTINGS)
	{

	}
	else if(state == STATE_PAUSED)
	{
		this->nrOfBtns = 3;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR3(midScreenW, midScreenH, 0), PAUSED_CONTINUE);
		this->menuBtns[1] = createBtn(D3DXVECTOR3(midScreenW, midScreenH + 0.15, 0), SETTINGS);
		this->menuBtns[2] = createBtn(D3DXVECTOR3(midScreenW, midScreenH + 0.3, 0), QUIT);
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
		temp.size = D3DXVECTOR3(20, 5, 0);
		temp.text = "New Game";
	}
	if(type == SETTINGS)
	{
		temp.size = D3DXVECTOR3(20, 5, 0);
		temp.text = "Settings";
	}
	if(type == QUIT)
	{
		temp.size = D3DXVECTOR3(10, 5, 0);
		temp.text = "Quit";
	}
	if(type == PAUSED_CONTINUE)
	{
		temp.size = D3DXVECTOR3(20, 5, 0);
		temp.text = "CONTINUE";
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