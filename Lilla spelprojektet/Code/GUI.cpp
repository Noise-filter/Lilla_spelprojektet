#include "GUI.h"

GUI::GUI()
{
	this->menuBtns = NULL;
	this->textBoxes = NULL;
	this->nrOfBtns = 0;
	this->nrOfBoxes = 0;
	this->GUI_STATE = 0;
	createBtns(STATE_MENU);


}

GUI::~GUI()
{
	clear();
}


void GUI::render(Button*& btns, Text*& text)
{
	if(GUI_STATE != STATE_GAMESTART || GUI_STATE != STATE_PLAYING || GUI_STATE != STATE_WIN || GUI_STATE != STATE_LOSE)
	{
		btns = this->menuBtns;
		text = this->textBoxes;
	}
	
}

int GUI::update(MouseState *mouse, int& state)
{
	if(GUI_STATE != state)
	{
		createBtns(state);
		GUI_STATE = state;
		//clear();
	}

	for(int i = 0; i < this->nrOfBtns; i++)
	{
		bool check = checkBtn(mouse, this->menuBtns[i]);
		if(check)
		{
			state = changeState(this->menuBtns[i]);
		}
	}
	return state;
}

void GUI::createBtns(int state)
{
	float midScreenW = (float)SCREEN_WIDTH/2;
	float midScreenH = (float)SCREEN_HEIGHT/2;


	if(this->menuBtns != NULL)
	{
		clear();
	}

	if(state == STATE_MENU)
	{
		this->nrOfBtns = 3;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR2(midScreenW, midScreenH - 30), STARTGAME);
		this->menuBtns[1] = createBtn(D3DXVECTOR2(midScreenW, midScreenH + 10), SETTINGS);
		this->menuBtns[2] = createBtn(D3DXVECTOR2(midScreenW, midScreenH + 50), QUIT);
		this->nrOfBoxes = 1;
		this->textBoxes = new Text[nrOfBoxes];
		this->textBoxes[0] = createTextBox(D3DXVECTOR2(midScreenW, midScreenH - 140), L"Game Name", 62, 0x800000ff);
	}
	else if(state == STATE_SETTINGS)
	{

	}
	else if(state == STATE_PAUSED)
	{
		this->nrOfBtns = 3;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR2(midScreenW, midScreenH), PAUSED_CONTINUE);
		this->menuBtns[1] = createBtn(D3DXVECTOR2(midScreenW, midScreenH + 0.15), SETTINGS);
		this->menuBtns[2] = createBtn(D3DXVECTOR2(midScreenW, midScreenH + 0.3), QUIT);
	}
	else if(state == STATE_NEWGAME)
	{
		this->nrOfBoxes = 2;
		this->textBoxes = new Text[nrOfBoxes];
		this->textBoxes[0] = createTextBox(D3DXVECTOR2(midScreenW, midScreenH - 100), L"Level 1", 36, 0x800000ff);
		this->textBoxes[1] = createTextBox(D3DXVECTOR2(midScreenW, midScreenH - 50), L"Easy", 36, 0x800000ff);
		this->nrOfBtns = 5;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR2(textBoxes[0].pos.x - 80, textBoxes[0].pos.y), LAST);
		this->menuBtns[1] = createBtn(D3DXVECTOR2(textBoxes[0].pos.x + 80, textBoxes[0].pos.y), NEXT);
		this->menuBtns[2] = createBtn(D3DXVECTOR2(textBoxes[1].pos.x - 80, textBoxes[1].pos.y), LAST);
		this->menuBtns[3] = createBtn(D3DXVECTOR2(textBoxes[1].pos.x + 80, textBoxes[1].pos.y), NEXT);
		this->menuBtns[4] = createBtn(D3DXVECTOR2(midScreenW, midScreenH + 75), STARTGAME);
	}
	
}

int GUI::getNrOfBtns()const
{
	return this->nrOfBtns;
}

int GUI::getNrOfText()const
{
	return this->nrOfBoxes;
}

void GUI::clear()
{
	delete this->menuBtns;
	this->menuBtns = NULL;
	this->nrOfBtns = 0;

	delete this->textBoxes;
	this->textBoxes = NULL;
	this->nrOfBoxes = 0;
}

Button GUI::createBtn(D3DXVECTOR2 pos, BUTTONTYPE type)
{
	Button btn;
	Text text;

	
	btn.pos = pos;
	btn.type = type;
	text.pos = pos;
	text.textSize = 36;
	text.textColor = 0x800000ff;

	if(type == STARTGAME)
	{
		btn.size = D3DXVECTOR2(50, 10);
		text.text = L"New Game";
	}
	if(type == SETTINGS)
	{
		btn.size = D3DXVECTOR2(50, 10);
		text.text = L"Settings";
	}
	if(type == QUIT)
	{
		btn.size = D3DXVECTOR2(50, 10);
		text.text = L"Quit";
	}
	if(type == PAUSED_CONTINUE)
	{
		btn.size = D3DXVECTOR2(50, 20);
		text.text = L"CONTINUE";
	}
	if(type == NEXT)
	{
		btn.size = D3DXVECTOR2(10, 10);
		text.text = L">";
	}
	if(type == LAST)
	{
		btn.size = D3DXVECTOR2(10, 10);
		text.text = L"<";
	}

	btn.text = text;

	return btn;
}

Text GUI::createTextBox(D3DXVECTOR2 pos, wchar_t* text, float size, UINT32 color)
{
	Text temp;
	temp.pos = pos;
	temp.text = text;
	temp.textSize = size;
	temp.textColor = color;
	return temp;
}

bool GUI::checkBtn(MouseState *mousePos, Button btn)
{
	if(mousePos->btnState == VK_LBUTTON)
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
	}
	return false;
}

int GUI::changeState(Button btn)
{
	int state = GUI_STATE;
	if(btn.type == STARTGAME && state == STATE_MENU)
	{
		state = STATE_NEWGAME;
	}
	else if(btn.type == STARTGAME && state == STATE_NEWGAME)
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