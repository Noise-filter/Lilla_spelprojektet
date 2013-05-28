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
	retry = false;
	first = true;
	createLevelList();
	this->currentLevel = 0;
	initDifficulty();
	this->currentBuilding = TEXTURE_MAINBUILDING;
	createBtns(STATE_MENU);
	createPanels(STATE_MENU);
	endStats = Statistics::Getinstance();
	
}

GUI::~GUI()
{

	clearBtns();
	clearTexts();
	clearPanels();
	SAFE_DELETE_ARRAY(levelList);
	this->nrOfLevels = 0;
	SAFE_DELETE_ARRAY(difficultyList);
	this->nrOfDifficultys = 0;
	endStats->shutdown();
}

void GUI::render(Button*& btns, Text*& text)
{
	
		btns = this->menuBtns;
		text = this->textBoxes;
		
	
}

int GUI::update(MouseState *mouse, int& state, bool& muted, bool& retry)
{
	if(GUI_STATE != state)
	{
		old_GUI_STATE = GUI_STATE;
		if(state == STATE_PAUSED && (old_GUI_STATE == STATE_GAMESTART || old_GUI_STATE == STATE_PLAYING))
		{
			lastPlayingState = GUI_STATE;
		}
		if(state != STATE_GAMESTART && state != STATE_PLAYING && old_GUI_STATE != STATE_PAUSED)
		{
			clearTexts();
		}
		clearBtns();
		clearPanels();
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
	retry = this->retry;
	return state;
}

void GUI::createBtns(int state)
{
	
	wchar_t* level = (wchar_t*)levelList[currentLevel].c_str();
	wchar_t* difficulty = (wchar_t*)difficultyList[currentDifficulty].c_str();

	if(state == STATE_MENU)
	{
		this->nrOfBtns = 3;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR2(0.5f*(0.3f*SCREEN_WIDTH),0.5f*(0.9f*SCREEN_HEIGHT)), STARTGAME);
		this->menuBtns[1] = createBtn(D3DXVECTOR2(0.5f*(0.3f*SCREEN_WIDTH),0.5f*(1.1f*SCREEN_HEIGHT)), SETTINGS);
		this->menuBtns[2] = createBtn(D3DXVECTOR2(0.5f*(0.3f*SCREEN_WIDTH),0.5f*(1.3f*SCREEN_HEIGHT)), QUIT);
	}
	else if(state == STATE_SETTINGS)
	{
		this->nrOfBtns = 2;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR2(0.5f*(1*SCREEN_WIDTH),0.5f*(0.55f*SCREEN_HEIGHT)), MUTE);
		this->menuBtns[1] = createBtn(D3DXVECTOR2(0.5f*(1*SCREEN_WIDTH),0.5f*(1.55f*SCREEN_HEIGHT)), BACK);
	}
	else if(state == STATE_PAUSED)
	{
		this->nrOfBtns = 3;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR2(0.5f*(0.3f*SCREEN_WIDTH),0.5f*(0.9f*SCREEN_HEIGHT)), PAUSED_CONTINUE);
		this->menuBtns[1] = createBtn(D3DXVECTOR2(0.5f*(0.3f*SCREEN_WIDTH),0.5f*(1.1f*SCREEN_HEIGHT)), SETTINGS);
		this->menuBtns[2] = createBtn(D3DXVECTOR2(0.5f*(0.3f*SCREEN_WIDTH),0.5f*(1.3f*SCREEN_HEIGHT)), MAIN_MENU);
		for(int i = 0; i < this->nrOfBtns; i++)
		{
			setLeftAligned(this->menuBtns[i].text);
		}
	}
	else if(state == STATE_NEWGAME)
	{
		this->nrOfBoxes = 2;
		this->textBoxes = new Text[nrOfBoxes];
		this->textBoxes[0] = createTextBox(D3DXVECTOR2(0.5f*(1*SCREEN_WIDTH),0.5f*(0.5f*SCREEN_HEIGHT)), level, 36, 0xffa8a8a8);
		this->textBoxes[1] = createTextBox(D3DXVECTOR2(0.5f*(1*SCREEN_WIDTH),0.5f*(0.65f*SCREEN_HEIGHT)), difficulty, 36, 0xffa8a8a8);
		this->nrOfBtns = 6;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR2(textBoxes[0].pos.x - 80, textBoxes[0].pos.y), LAST);
		this->menuBtns[1] = createBtn(D3DXVECTOR2(textBoxes[0].pos.x + 80, textBoxes[0].pos.y), NEXT);
		this->menuBtns[2] = createBtn(D3DXVECTOR2(textBoxes[1].pos.x - 80, textBoxes[1].pos.y), LAST);
		this->menuBtns[3] = createBtn(D3DXVECTOR2(textBoxes[1].pos.x + 80, textBoxes[1].pos.y), NEXT);
		this->menuBtns[4] = createBtn(D3DXVECTOR2(0.5f*(1*SCREEN_WIDTH),0.5f*(1.45f*SCREEN_HEIGHT)), STARTGAME);
		this->menuBtns[5] = createBtn(D3DXVECTOR2(0.5f*(1*SCREEN_WIDTH),0.5f*(1.6f*SCREEN_HEIGHT)), BACK);
	}
	else if(state == STATE_GAMESTART || state == STATE_PLAYING)
	{
		if(first)
		{
		this->nrOfBoxes+=3;
		first = false;
		}
		Text* temp = new Text[this->nrOfBoxes];
		for(int i = 0; i < this->nrOfBoxes-1; i++)
		{
			temp[i] = this->textBoxes[i];
		}
		delete []this->textBoxes;
		this->textBoxes = NULL;
		this->textBoxes = temp;
		this->textBoxes[8] = createTextBox(D3DXVECTOR2(0.5f*(0.8f*SCREEN_WIDTH),0.5f*(1.7f*SCREEN_HEIGHT)), L"Resource:", 18, 0xffffffff);
		this->textBoxes[9] = createTextBox(D3DXVECTOR2(0.5f*(0.775f*SCREEN_WIDTH),0.5f*(1.875f*SCREEN_HEIGHT)), L"Supply:", 18, 0xffffffff);
		this->textBoxes[10] = createTextBox(D3DXVECTOR2(0.5f*(1.2f*SCREEN_WIDTH),0.5f*(1.7f*SCREEN_HEIGHT)), L"Time:", 18, 0xffffffff);
	}
	else if(state == STATE_WIN)
	{
		
		getEndStats();
		this->nrOfBoxes+=1;
		Text* temp = new Text[this->nrOfBoxes];
		for(int i = 0; i < this->nrOfBoxes-1; i++)
		{
			temp[i] = this->textBoxes[i];
		}
		delete []this->textBoxes;
		this->textBoxes = NULL;
		this->textBoxes = temp;
		for(int i = 0; i < 7; i++)
		{
			setLeftAligned(this->textBoxes[i]);
		}
		this->textBoxes[7] = createTextBox(D3DXVECTOR2(0.5f*(1*SCREEN_WIDTH),0.5f*(0.55f*SCREEN_HEIGHT)), L"You Won!", 62, 0xffa8a8a8);
		this->nrOfBtns =2;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR2(0.5f*(0.8f*SCREEN_WIDTH),0.5f*(1.6f*SCREEN_HEIGHT)), MAIN_MENU);
		this->menuBtns[1] = createBtn(D3DXVECTOR2(0.5f*(1.2f*SCREEN_WIDTH),0.5f*(1.6f*SCREEN_HEIGHT)), RETRY);
	}
	else if(state == STATE_LOSE)
	{
		getEndStats();
		this->nrOfBoxes+=1;
		Text* temp = new Text[this->nrOfBoxes];
		for(int i = 0; i < this->nrOfBoxes-1; i++)
		{
			temp[i] = this->textBoxes[i];
		}
		delete []this->textBoxes;
		this->textBoxes = NULL;
		this->textBoxes = temp;
		for(int i = 0; i < 7; i++)
		{
			setLeftAligned(this->textBoxes[i]);
		}
		this->textBoxes[7] = createTextBox(D3DXVECTOR2(0.5f*(1*SCREEN_WIDTH),0.5f*(0.55f*SCREEN_HEIGHT)), L"You Lost", 62, 0xffa8a8a8);
		this->nrOfBtns = 2;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR2(0.5f*(0.8f*SCREEN_WIDTH),0.5f*(1.6f*SCREEN_HEIGHT)), MAIN_MENU);
		this->menuBtns[1] = createBtn(D3DXVECTOR2(0.5f*(1.2f*SCREEN_WIDTH),0.5f*(1.6f*SCREEN_HEIGHT)), RETRY);
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

void GUI::clearBtns()
{
	SAFE_DELETE_ARRAY(menuBtns);
	this->nrOfBtns = 0;
}

void GUI::clearTexts()
{
	SAFE_DELETE_ARRAY(textBoxes);
	this->nrOfBoxes = 0;
}

void GUI::clearPanels()
{
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
	text.center = true;
	text.textColor = 0xffa8a8a8;

	if(type == STARTGAME)
	{
		btn.size = D3DXVECTOR2(90, 15);
		text.text = L"New Game";
	}
	if(type == SETTINGS)
	{
		btn.size = D3DXVECTOR2(65, 18);
		text.text = L"Settings";
	}
	if(type == QUIT)
	{
		btn.size = D3DXVECTOR2(40, 18);
		text.text = L"Quit";
	}
	if(type == PAUSED_CONTINUE)
	{
		btn.size = D3DXVECTOR2(80, 20);
		text.text = L"Continue";
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
	if(type == BACK)
	{
		btn.size = D3DXVECTOR2(40, 20);
		text.text = L"Back";
	}
	if(type == MUTE)
	{
		btn.size = D3DXVECTOR2(40, 20);
		text.text = L"Mute";
	}
	if(type == MAIN_MENU)
	{
		btn.size = D3DXVECTOR2(40, 20);
		text.text = L"Menu";
	}
	if(type == RETRY)
	{
		btn.size = D3DXVECTOR2(40, 20);
		text.text = L"Retry";
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
	temp.center = true;
	return temp;
}

void GUI::createPanels(int state)
{
	if(state == STATE_MENU || state == SETTINGS || state == STATE_NEWGAME || state == STATE_PAUSED || state == STATE_WIN || state == STATE_LOSE)
	{
		this->nrOfPanles = 1;
		this->panels = new GUI_Panel[nrOfPanles];
		this->panels[0] = GUI_Panel(D3DXVECTOR2(0, 0), D3DXVECTOR2(1, 1), TEXTURE_MENU);
	}
	else if(state == STATE_GAMESTART || state == STATE_PLAYING )
	{
		this->nrOfPanles = 7;
		this->panels = new GUI_Panel[nrOfPanles];
		this->panels[0] = GUI_Panel(D3DXVECTOR2(-0.8225f, -0.8f), D3DXVECTOR2(0.14f, 0.14f), currentBuilding);
		this->panels[1] = GUI_Panel(D3DXVECTOR2(0.188f, -0.89f), D3DXVECTOR2(0.07f, 0.08f), TEXTURE_SUPPLY_TINY);
		this->panels[2] = GUI_Panel(D3DXVECTOR2(0.365f, -0.89f), D3DXVECTOR2(0.07f, 0.08f), TEXTURE_TOWER_TINY);
		this->panels[3] = GUI_Panel(D3DXVECTOR2(0.545f, -0.89f), D3DXVECTOR2(0.07f, 0.08f), TEXTURE_ATK_TINY);
		this->panels[4] = GUI_Panel(D3DXVECTOR2(0.7225f, -0.89f), D3DXVECTOR2(0.07f, 0.08f), TEXTURE_DEF_TINY);
		this->panels[5] = GUI_Panel(D3DXVECTOR2(0.8975f, -0.89f), D3DXVECTOR2(0.07f, 0.08f), TEXTURE_MONEY_TINY);
		this->panels[6] = GUI_Panel(D3DXVECTOR2(0, -0.8f), D3DXVECTOR2(1, 0.2f), TEXTURE_FRAME);
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
		state = lastPlayingState;
		first = true;
	}
	else if(btn.type == BACK)
	{
		state = old_GUI_STATE;
	}
	else if(btn.type == MAIN_MENU)
	{
		 state = STATE_MENU;
		 first = true;
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
	else if(btn.type == RETRY)
	{
		state = STATE_GAMESTART;
		this->retry = true;
		first = true;
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
		for(int i = 0; i < (int)attribute.size(); i++)
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

	textBoxes[0].text = (wchar_t*)levelList[currentLevel].c_str();

	if(pos.y == textBoxes[1].pos.y && type == NEXT)
	{
		this->currentDifficulty = (currentDifficulty+1) % nrOfDifficultys;
	}
	else if(pos.y == textBoxes[1].pos.y && type == LAST)
	{
		this->currentDifficulty--;
		if(this->currentDifficulty < 0)
		{
			this->currentDifficulty = this->nrOfDifficultys-1;
		}
	}
	wchar_t* difficulty = (wchar_t*)difficultyList[currentDifficulty].c_str();
	textBoxes[1].text = difficulty;
}

string GUI::getCurrentLevel()const
{
	string temp;
	for(int i = 0; i < (int)levelList[currentLevel].size(); i++)
	{
		temp.push_back((char)levelList[currentLevel][i]);
	}

	return temp;
}

void GUI::initDifficulty()
{
	this->nrOfDifficultys = 3;
	this->difficultyList = new wstring[nrOfDifficultys];
	this->difficultyList[0] = L"Easy";
	this->difficultyList[1] = L"Medium";
	this->difficultyList[2] = L"Hard";
	this->currentDifficulty = 0;
}

int GUI::getCurrentDiff()const
{
	return this->currentDifficulty;
}

wstring GUI::convertStrToWstr(string text)
{
	wstring temp(text.begin(), text.end());
	//copy(text.begin(), text.end(), temp.begin());
	return temp;
}

string GUI::convertWstrToStr(wstring text)
{
	string temp(text.begin(), text.end());
    //copy(text.begin(), text.end(), temp.begin());
    return temp;
}

void GUI::getEndStats()
{
	int nrOfStats = 7;
	this->nrOfBoxes = nrOfStats;
	this->textBoxes = new Text[this->nrOfBoxes];
	char temp[255];
	string Stats[7];
	Stats[0] = "Level: " + getCurrentLevel();
	Stats[1] = "Resources: ";
	Stats[1].append( _itoa(this->endStats->totalRes, temp, 10) );
	Stats[2] = "Supplys: ";
	Stats[2].append (_itoa(this->endStats->totalSupply, temp, 10) );
	Stats[3] = "Time: ";
	Stats[3].append(_itoa((int)this->endStats->totalTime, temp, 10) );
	Stats[4] = "Buildings built: ";
	Stats[4].append(_itoa(this->endStats->totalNrOfBuildings, temp, 10) );
	Stats[5] = "Enemies killed: ";
	Stats[5].append(_itoa(this->endStats->totalEnemiesKilled, temp, 10) );
	Stats[6] = "Nr of Upgrades: ";
	Stats[6].append(_itoa(this->endStats->totalNrOfUpgrades, temp, 10) );
	
	
	for(int i = 0; i < nrOfStats; i++)
	{
		wStats[i] = convertStrToWstr(Stats[i]);
	}

	this->textBoxes[0] = createTextBox(D3DXVECTOR2(0.5f*(0.4f*SCREEN_WIDTH),0.5f*(0.75f*SCREEN_HEIGHT)), (wchar_t*)wStats[0].c_str(), 18, 0xffffffff);
	this->textBoxes[1] = createTextBox(D3DXVECTOR2(0.5f*(0.4f*SCREEN_WIDTH),0.5f*(0.85f*SCREEN_HEIGHT)), (wchar_t*)wStats[1].c_str(), 18, 0xffffffff);
	this->textBoxes[2] = createTextBox(D3DXVECTOR2(0.5f*(0.4f*SCREEN_WIDTH),0.5f*(0.95f*SCREEN_HEIGHT)), (wchar_t*)wStats[2].c_str(), 18, 0xffffffff);
	this->textBoxes[3] = createTextBox(D3DXVECTOR2(0.5f*(0.4f*SCREEN_WIDTH),0.5f*(1.05f*SCREEN_HEIGHT)), (wchar_t*)wStats[3].c_str(), 18, 0xffffffff);
	this->textBoxes[4] = createTextBox(D3DXVECTOR2(0.5f*(0.4f*SCREEN_WIDTH),0.5f*(1.15f*SCREEN_HEIGHT)), (wchar_t*)wStats[4].c_str(), 18, 0xffffffff);
	this->textBoxes[5] = createTextBox(D3DXVECTOR2(0.5f*(0.4f*SCREEN_WIDTH),0.5f*(1.25f*SCREEN_HEIGHT)), (wchar_t*)wStats[5].c_str(), 18, 0xffffffff);
	this->textBoxes[6] = createTextBox(D3DXVECTOR2(0.5f*(0.4f*SCREEN_WIDTH),0.5f*(1.35f*SCREEN_HEIGHT)), (wchar_t*)wStats[6].c_str(), 18, 0xffffffff);

}

string GUI::convertFloat(float value) 
{
	std::ostringstream o;
	if (!(o << value))
		return "";
  return o.str();
}

void GUI::setLeftAligned(Text& text)
{
	text.center = false;
}

void GUI::setInGameText(int resource, int supply, int nrOfBuildings, int goal, int selectedStructure)
{
	string name;
	string purpose;
	int cost;
	string typeOfCost;
	if(selectedStructure == BUILDABLE_MAINBUILDING)
	{
		name = "Headquarter";
		purpose = "Base";
		cost = 0;
		currentBuilding = TEXTURE_MAINBUILDING;
		typeOfCost = "Cost: ";
	}
	else if(selectedStructure == BUILDABLE_SUPPLY)
	{
		name = "Supply";
		purpose = "Give supply";
		cost = COST_SUPPLY;
		currentBuilding = TEXTURE_SUPPLY;
		typeOfCost = "Resource: ";
	}
	else if(selectedStructure == BUILDABLE_TOWER)
	{
		name = "Tower";
		purpose = "Offensive structure";
		cost = COST_TOWER;
		currentBuilding = TEXTURE_TOWER;
		typeOfCost = "Supply: ";
	}
	else if(selectedStructure == BUILDABLE_UPGRADE_OFFENSE)
	{
		name = "Offensive";
		purpose = "Upgrade";
		cost = COST_UPGRADE;
		currentBuilding = TEXTURE_ATK;
		typeOfCost = "Resource: ";
	}
	else if(selectedStructure == BUILDABLE_UPGRADE_DEFENSE)
	{
		name = "Defensive";
		purpose = "Upgrade";
		cost = COST_UPGRADE;
		currentBuilding = TEXTURE_DEF;
		typeOfCost = "Resource: ";
	}
	else if(selectedStructure == BUILDABLE_UPGRADE_RES)
	{
		name = "Resources";
		purpose = "Increase res/kill";
		cost = COST_UPGRADE;
		currentBuilding = TEXTURE_MONEY;
		typeOfCost = "Resource: ";
	}
		
	if(first)
	{
		this->nrOfBoxes = 8;
		this->textBoxes = new Text[this->nrOfBoxes];
		
	}
	char temp[255];
	string Stats[8];
	Stats[0] = _itoa(resource, temp, 10);
	Stats[1] = _itoa(supply, temp, 10);
	Stats[2] = _itoa((int)this->endStats->totalTime, temp, 10);
	Stats[3] = "Built: ";
	Stats[3].append( _itoa(nrOfBuildings, temp, 10));
	Stats[4] = "Goal: ";
	Stats[4].append(_itoa(goal, temp, 10));
	Stats[5] = name;
	Stats[6] = purpose;
	Stats[7] = typeOfCost;
	Stats[7].append(_itoa(cost, temp, 10) );
	

	for(int i = 0; i < 8; i++)
	{
		wStats[i] = convertStrToWstr(Stats[i]);
	}

	this->textBoxes[0] = createTextBox(D3DXVECTOR2(0.5f*(1*SCREEN_WIDTH),0.5f*(1.7f*SCREEN_HEIGHT)), (wchar_t*)wStats[0].c_str(), 18, 0xffffffff);
	this->textBoxes[1] = createTextBox(D3DXVECTOR2(0.5f*(1*SCREEN_WIDTH),0.5f*(1.875f*SCREEN_HEIGHT)), (wchar_t*)wStats[1].c_str(), 18, 0xffffffff);
	this->textBoxes[2] = createTextBox(D3DXVECTOR2(0.5f*(1.3f*SCREEN_WIDTH),0.5f*(1.7f*SCREEN_HEIGHT)), (wchar_t*)wStats[2].c_str(), 18, 0xffffffff);
	this->textBoxes[3] = createTextBox(D3DXVECTOR2(0.5f*(1.65f*SCREEN_WIDTH),0.5f*(1.7f*SCREEN_HEIGHT)), (wchar_t*)wStats[3].c_str(), 18, 0xffffffff);
	this->textBoxes[4] = createTextBox(D3DXVECTOR2(0.5f*(1.85f*SCREEN_WIDTH),0.5f*(1.7f*SCREEN_HEIGHT)), (wchar_t*)wStats[4].c_str(), 18, 0xffffffff);
	this->textBoxes[5] = createTextBox(D3DXVECTOR2(0.5f*(0.5f*SCREEN_WIDTH),0.5f*(1.7f*SCREEN_HEIGHT)), (wchar_t*)wStats[5].c_str(), 18, 0xffffffff);
	this->textBoxes[6] = createTextBox(D3DXVECTOR2(0.5f*(0.51f*SCREEN_WIDTH),0.5f*(1.8f*SCREEN_HEIGHT)), (wchar_t*)wStats[6].c_str(), 14, 0xffffffff);
	this->textBoxes[7] = createTextBox(D3DXVECTOR2(0.5f*(0.5f*SCREEN_WIDTH),0.5f*(1.9f*SCREEN_HEIGHT)), (wchar_t*)wStats[7].c_str(), 14, 0xffffffff);

	this->panels[0].textureID = currentBuilding;
}

void GUI::restartStats()
{
	endStats->shutdown();
	endStats = endStats->Getinstance();
}