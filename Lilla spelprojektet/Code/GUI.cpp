#include "GUI.h"

GUI::GUI()
{
	this->menuBtns = NULL;
	this->textBoxes = NULL;
	this->panels = NULL; 
	this->nrOfBtns = 0;
	this->nrOfBoxes = 0;
	this->nrOfPanles = 0;
	this->GUI_STATE = 0;
	midScreenW = (float)SCREEN_WIDTH/2;
	midScreenH = (float)SCREEN_HEIGHT/2;
	muted = false;
	createLevelList();
	this->currentLevel = 0;
	
	createBtns(STATE_MENU);
}

GUI::~GUI()
{

	clear();
	SAFE_DELETE_ARRAY(levelList);
	this->nrOfLevels = 0;

}


void GUI::render(Button*& btns, Text*& text)
{
	
		btns = this->menuBtns;
		text = this->textBoxes;
		
	
}

int GUI::update(MouseState *mouse, int& state, bool& muted)
{
	if(GUI_STATE != state)
	{
		createBtns(state);
		createPanels(state);
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
	muted = this->muted;
	return state;
}

void GUI::createBtns(int state)
{

	//if(this->menuBtns != NULL)
	{
		clear();
	}
	wchar_t* level = (wchar_t*)levelList[currentLevel].c_str();

	if(state == STATE_MENU)
	{
		this->nrOfBtns = 3;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR2(midScreenW, midScreenH - 30), STARTGAME);
		this->menuBtns[1] = createBtn(D3DXVECTOR2(midScreenW, midScreenH + 10), SETTINGS);
		this->menuBtns[2] = createBtn(D3DXVECTOR2(midScreenW, midScreenH + 50), QUIT);
		this->nrOfBoxes = 1;
		this->textBoxes = new Text[nrOfBoxes];
		this->textBoxes[0] = createTextBox(D3DXVECTOR2(midScreenW, midScreenH - 140), L".exe", 62, 0x800000ff);
	}
	else if(state == STATE_SETTINGS)
	{
		this->nrOfBtns = 2;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR2(midScreenW, midScreenH - 30), MUTE);
		this->menuBtns[1] = createBtn(D3DXVECTOR2(midScreenW, midScreenH + 120), MAIN_MENU);
	}
	else if(state == STATE_PAUSED)
	{
		this->nrOfBtns = 3;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR2(midScreenW, midScreenH), PAUSED_CONTINUE);
		this->menuBtns[1] = createBtn(D3DXVECTOR2(midScreenW, midScreenH + 30), SETTINGS);
		this->menuBtns[2] = createBtn(D3DXVECTOR2(midScreenW, midScreenH + +60), MAIN_MENU);
	}
	else if(state == STATE_NEWGAME)
	{
		this->nrOfBoxes = 2;
		this->textBoxes = new Text[nrOfBoxes];
		this->textBoxes[0] = createTextBox(D3DXVECTOR2(midScreenW, midScreenH - 100), level, 36, 0x800000ff);
		this->textBoxes[1] = createTextBox(D3DXVECTOR2(midScreenW, midScreenH - 50), L"Easy", 36, 0x800000ff);
		this->nrOfBtns = 6;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR2(textBoxes[0].pos.x - 80, textBoxes[0].pos.y), LAST);
		this->menuBtns[1] = createBtn(D3DXVECTOR2(textBoxes[0].pos.x + 80, textBoxes[0].pos.y), NEXT);
		this->menuBtns[2] = createBtn(D3DXVECTOR2(textBoxes[1].pos.x - 80, textBoxes[1].pos.y), LAST);
		this->menuBtns[3] = createBtn(D3DXVECTOR2(textBoxes[1].pos.x + 80, textBoxes[1].pos.y), NEXT);
		this->menuBtns[4] = createBtn(D3DXVECTOR2(midScreenW, midScreenH + 75), STARTGAME);
		this->menuBtns[5] = createBtn(D3DXVECTOR2(midScreenW, midScreenH + 120), MAIN_MENU);
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

GUI_Panel* GUI::getPanels()const
{
	return this->panels;
}

int GUI::getNrOfPanels()const
{
	return this->nrOfPanles;
}

void GUI::clear()
{
	SAFE_DELETE_ARRAY(menuBtns);
	this->nrOfBtns = 0;

	SAFE_DELETE_ARRAY(textBoxes);
	this->nrOfBoxes = 0;

	SAFE_DELETE_ARRAY(panels);
	this->nrOfPanles = 0;
}

Button GUI::createBtn(D3DXVECTOR2 pos, BUTTONTYPE type)
{
	Button btn;
	Text text;
	
	btn.pos = pos;
	btn.type = type;
	text.pos = pos;
	text.textSize = 36;
	//text.pos.y = pos.y;
	text.textColor = 0x800000ff;

	if(type == STARTGAME)
	{
		btn.size = D3DXVECTOR2(100, 15);
		text.text = L"New Game";
	}
	if(type == SETTINGS)
	{
		btn.size = D3DXVECTOR2(100, 18);
		text.text = L"Settings";
	}
	if(type == QUIT)
	{
		btn.size = D3DXVECTOR2(80, 18);
		text.text = L"Quit";
	}
	if(type == PAUSED_CONTINUE)
	{
		btn.size = D3DXVECTOR2(100, 20);
		text.text = L"CONTINUE";
	}
	if(type == NEXT)
	{
		btn.size = D3DXVECTOR2(20, 20);
		text.text = L">";
	}
	if(type == LAST)
	{
		btn.size = D3DXVECTOR2(20, 20);
		text.text = L"<";
	}
	if(type == MAIN_MENU)
	{
		btn.size = D3DXVECTOR2(80, 20);
		text.text = L"Main Menu";
	}
	if( type == MUTE)
	{
		btn.size = D3DXVECTOR2(80, 20);
		text.text = L"Mute";
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

void GUI::createPanels(int state)
{
	if(state == STATE_MENU || state == SETTINGS || state == STATE_NEWGAME)
	{
		this->nrOfPanles = 1;
		this->panels = new GUI_Panel[nrOfPanles];
		this->panels[0] = GUI_Panel(D3DXVECTOR2(0, 0), D3DXVECTOR2(1, 1), TEXTURE_MAIN_MENU);
	}
	else if(state == STATE_GAMESTART || state == STATE_PLAYING )
	{
		this->nrOfPanles = 5;
		this->panels = new GUI_Panel[nrOfPanles];
		this->panels[0] = GUI_Panel(D3DXVECTOR2(-0.8, 0.8), D3DXVECTOR2(0.1, 0.1), TEXTURE_RESOURSE);
		this->panels[1] = GUI_Panel(D3DXVECTOR2(0.8, 0.8), D3DXVECTOR2(0.1, 0.1), TEXTURE_GOAL);
		this->panels[2] = GUI_Panel(D3DXVECTOR2(0.8, -0.8), D3DXVECTOR2(0.1, 0.1), TEXTURE_BUILDINGS_HOTKEY);
		this->panels[3] = GUI_Panel(D3DXVECTOR2(-0.8, -0.8), D3DXVECTOR2(0.1, 0.1), TEXTURE_INFO);
		this->panels[4] = GUI_Panel(D3DXVECTOR2(-0.9, -0.9), D3DXVECTOR2(0.1, 0.1), TEXTURE_BUILDING);
	}
	else if(state == STATE_WIN || state == STATE_LOSE)
	{

	}
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
	else if(btn.type == MAIN_MENU)
	{
		state = STATE_MENU;
	}
	else if(btn.type == QUIT)
	{
		state = STATE_QUIT;
	}
	else if(btn.type == MUTE)
	{
		muted = !muted;
	}
	else if(btn.type == NEXT || btn.type == LAST)
	{
		changeText(btn.pos, btn.type);
	}
	return state;
}

void GUI::createLevelList()
{
	ifstream fin;
	fin.open("Level list.txt");
	string attribute;
	int value;

	if(fin.fail() == true)
	{
		cout << "FAILED TO READ LEVELS FROM FILE" << endl;
	}

	fin >> value;
	this->nrOfLevels = value;
	this->levelList = new wstring[nrOfLevels];
	int count = 0;
	while(count != nrOfLevels)
	{
		fin >> attribute;
		for(int i = 0; i < attribute.size(); i++)
		{
			this->levelList[count].push_back(attribute[i]);
		}
		//this->levelList[count] = wstring(attribute.begin(), attribute.end());

		count++;
	}

	//läs från fil istället

	fin.close();
}

void GUI::changeText(D3DXVECTOR2 pos, BUTTONTYPE type)
{
	if(pos.y == textBoxes[0].pos.y && type == NEXT)
	{
		this->currentLevel = (currentLevel+1) % nrOfLevels;
	}
	else if(pos.y == textBoxes[0].pos.y && type == LAST)
	{
		this->currentLevel--;
		if(this->currentLevel < 0)
		{
			this->currentLevel = this->nrOfLevels-1;
		}
	}
	wchar_t* level = (wchar_t*)levelList[currentLevel].c_str();
	textBoxes[0].text = level;
}

string GUI::getCurrentLevel()const
{
	string temp;
	for(int i = 0; i < levelList[currentLevel].size(); i++)
	{
		temp.push_back(levelList[currentLevel][i]);
	}

	return temp;
}