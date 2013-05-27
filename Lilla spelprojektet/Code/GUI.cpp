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
		if(state != STATE_GAMESTART && state != STATE_PLAYING && old_GUI_STATE != STATE_PAUSED && old_GUI_STATE != STATE_WIN && old_GUI_STATE != STATE_LOSE)
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
		this->menuBtns[0] = createBtn(D3DXVECTOR2(0.5*(0.3*SCREEN_WIDTH),0.5*(0.9*SCREEN_HEIGHT)), STARTGAME);
		this->menuBtns[1] = createBtn(D3DXVECTOR2(0.5*(0.3*SCREEN_WIDTH),0.5*(1.1*SCREEN_HEIGHT)), SETTINGS);
		this->menuBtns[2] = createBtn(D3DXVECTOR2(0.5*(0.3*SCREEN_WIDTH),0.5*(1.3*SCREEN_HEIGHT)), QUIT);
		for(int i = 0; i < this->nrOfBtns; i++)
		{
			setLeftAligned(this->menuBtns[i].text);
		}
		//this->nrOfBoxes = 1;
		//this->textBoxes = new Text[nrOfBoxes];
		//this->textBoxes[0] = createTextBox(D3DXVECTOR2(midScreenW, midScreenH - 140), L"Caecus", 62, 0x800000ff);
	}
	else if(state == STATE_SETTINGS)
	{
		this->nrOfBtns = 2;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR2(0.5*(1*SCREEN_WIDTH),0.5*(0.55*SCREEN_HEIGHT)), MUTE);
		this->menuBtns[1] = createBtn(D3DXVECTOR2(0.5*(1*SCREEN_WIDTH),0.5*(1.55*SCREEN_HEIGHT)), BACK);
	}
	else if(state == STATE_PAUSED)
	{
		this->nrOfBtns = 3;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR2(0.5*(0.3*SCREEN_WIDTH),0.5*(0.9*SCREEN_HEIGHT)), PAUSED_CONTINUE);
		this->menuBtns[1] = createBtn(D3DXVECTOR2(0.5*(0.3*SCREEN_WIDTH),0.5*(1.1*SCREEN_HEIGHT)), SETTINGS);
		this->menuBtns[2] = createBtn(D3DXVECTOR2(0.5*(0.3*SCREEN_WIDTH),0.5*(1.3*SCREEN_HEIGHT)), MAIN_MENU);
		for(int i = 0; i < this->nrOfBtns; i++)
		{
			setLeftAligned(this->menuBtns[i].text);
		}
	}
	else if(state == STATE_NEWGAME)
	{
		this->nrOfBoxes = 2;
		this->textBoxes = new Text[nrOfBoxes];
		this->textBoxes[0] = createTextBox(D3DXVECTOR2(0.5*(1*SCREEN_WIDTH),0.5*(0.5*SCREEN_HEIGHT)), level, 36, 0xffa8a8a8);
		this->textBoxes[1] = createTextBox(D3DXVECTOR2(0.5*(1*SCREEN_WIDTH),0.5*(0.65*SCREEN_HEIGHT)), difficulty, 36, 0xffa8a8a8);
		this->nrOfBtns = 6;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR2(textBoxes[0].pos.x - 80, textBoxes[0].pos.y), LAST);
		this->menuBtns[1] = createBtn(D3DXVECTOR2(textBoxes[0].pos.x + 80, textBoxes[0].pos.y), NEXT);
		this->menuBtns[2] = createBtn(D3DXVECTOR2(textBoxes[1].pos.x - 80, textBoxes[1].pos.y), LAST);
		this->menuBtns[3] = createBtn(D3DXVECTOR2(textBoxes[1].pos.x + 80, textBoxes[1].pos.y), NEXT);
		this->menuBtns[4] = createBtn(D3DXVECTOR2(0.5*(1*SCREEN_WIDTH),0.5*(1.45*SCREEN_HEIGHT)), STARTGAME);
		this->menuBtns[5] = createBtn(D3DXVECTOR2(0.5*(1*SCREEN_WIDTH),0.5*(1.6*SCREEN_HEIGHT)), BACK);
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
		//this->textBoxes[5] = createTextBox(D3DXVECTOR2(0.5*(0.5*SCREEN_WIDTH),0.5*(1.7*SCREEN_HEIGHT)), L"Information:", 18, 0xffffffff);
		this->textBoxes[8] = createTextBox(D3DXVECTOR2(0.5*(0.8*SCREEN_WIDTH),0.5*(1.7*SCREEN_HEIGHT)), L"Resource:", 18, 0xffffffff);
		this->textBoxes[9] = createTextBox(D3DXVECTOR2(0.5*(0.775*SCREEN_WIDTH),0.5*(1.875*SCREEN_HEIGHT)), L"Supply:", 18, 0xffffffff);
		this->textBoxes[10] = createTextBox(D3DXVECTOR2(0.5*(1.2*SCREEN_WIDTH),0.5*(1.7*SCREEN_HEIGHT)), L"Time:", 18, 0xffffffff);
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
		for(int i = 0; i < 8; i++)
		{
			setLeftAligned(this->textBoxes[i]);
		}
		this->textBoxes[8] = createTextBox(D3DXVECTOR2(0.5*(1*SCREEN_WIDTH),0.5*(0.45*SCREEN_HEIGHT)), L"You Won!", 62, 0xffa8a8a8);
		this->nrOfBtns =2;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR2(0.5*(0.8*SCREEN_WIDTH),0.5*(1.6*SCREEN_HEIGHT)), MAIN_MENU);
		this->menuBtns[1] = createBtn(D3DXVECTOR2(0.5*(1.2*SCREEN_WIDTH),0.5*(1.6*SCREEN_HEIGHT)), RETRY);
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
		for(int i = 0; i < 8; i++)
		{
			setLeftAligned(this->textBoxes[i]);
		}
		this->textBoxes[8] = createTextBox(D3DXVECTOR2(0.5*(1*SCREEN_WIDTH),0.5*(0.45*SCREEN_HEIGHT)), L"You Lost", 62, 0xffa8a8a8);
		this->nrOfBtns = 2;
		this->menuBtns = new Button[nrOfBtns];
		this->menuBtns[0] = createBtn(D3DXVECTOR2(0.5*(0.8*SCREEN_WIDTH),0.5*(1.6*SCREEN_HEIGHT)), MAIN_MENU);
		this->menuBtns[1] = createBtn(D3DXVECTOR2(0.5*(1.2*SCREEN_WIDTH),0.5*(1.6*SCREEN_HEIGHT)), RETRY);
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
		btn.size = D3DXVECTOR2(80, 20);
		text.text = L"Back";
	}
	if(type == MUTE)
	{
		btn.size = D3DXVECTOR2(80, 20);
		text.text = L"Mute";
	}
	if(type == MAIN_MENU)
	{
		btn.size = D3DXVECTOR2(100, 20);
		text.text = L"Main Menu";
	}
	if(type == RETRY)
	{
		btn.size = D3DXVECTOR2(80, 20);
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
		this->panels[0] = GUI_Panel(D3DXVECTOR2(-0.8225, -0.8), D3DXVECTOR2(0.13, 0.13), currentBuilding);
		this->panels[1] = GUI_Panel(D3DXVECTOR2(0.188, -0.89), D3DXVECTOR2(0.07, 0.07), TEXTURE_SUPPLY_TINY);
		this->panels[2] = GUI_Panel(D3DXVECTOR2(0.365, -0.89), D3DXVECTOR2(0.07, 0.07), TEXTURE_TOWER_TINY);
		this->panels[3] = GUI_Panel(D3DXVECTOR2(0.545, -0.89), D3DXVECTOR2(0.07, 0.07), TEXTURE_ATK_TINY);
		this->panels[4] = GUI_Panel(D3DXVECTOR2(0.7225, -0.89), D3DXVECTOR2(0.07, 0.07), TEXTURE_DEF_TINY);
		this->panels[5] = GUI_Panel(D3DXVECTOR2(0.8975, -0.89), D3DXVECTOR2(0.07, 0.07), TEXTURE_MONEY_TINY);
		this->panels[6] = GUI_Panel(D3DXVECTOR2(0, -0.8), D3DXVECTOR2(1, 0.2), TEXTURE_FRAME);
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
		temp.push_back(levelList[currentLevel][i]);
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
	int nrOfStats = 8;
	this->nrOfBoxes = nrOfStats;
	this->textBoxes = new Text[this->nrOfBoxes];
	char temp[255];
	string Stats[8];
	Stats[0] = "Level: " + getCurrentLevel();
	Stats[1] = "Score: ";
	Stats[1].append( itoa(this->endStats->totalScore, temp, 10) );
	Stats[2] = "Resources: ";
	Stats[2].append( itoa(this->endStats->totalRes, temp, 10) );
	Stats[3] = "Supplys: ";
	Stats[3].append (itoa(this->endStats->totalSupply, temp, 10) );
	Stats[4] = "Time: ";
	Stats[4].append( itoa(this->endStats->totalTime, temp, 10) );
	Stats[5] = "Buildings built: ";
	Stats[5].append( itoa(this->endStats->totalNrOfBuildings, temp, 10) );
	Stats[6] = "Enemies killed: ";
	Stats[6].append( itoa(this->endStats->totalEnemiesKilled, temp, 10) );
	Stats[7] = "Nr of Upgrades: ";
	Stats[7].append( itoa(this->endStats->totalNrOfUpgrades, temp, 10) );
	//Stats[8] = "Buildings at max lv: ";
	//Stats[8].append( itoa(this->endStats->totalNrOfMaxLvlTowers, temp, 10) );
	//Stats[9] = "Average lv: ";
	//Stats[9].append( convertFloat(this->endStats->averageTowerLvl) );

	
	for(int i = 0; i < nrOfStats; i++)
	{
		wStats[i] = convertStrToWstr(Stats[i]);
	}

	this->textBoxes[0] = createTextBox(D3DXVECTOR2(0.5*(0.4*SCREEN_WIDTH),0.5*(0.75*SCREEN_HEIGHT)), (wchar_t*)wStats[0].c_str(), 18, 0xffffffff);
	this->textBoxes[1] = createTextBox(D3DXVECTOR2(0.5*(0.4*SCREEN_WIDTH),0.5*(0.85*SCREEN_HEIGHT)), (wchar_t*)wStats[1].c_str(), 18, 0xffffffff);
	this->textBoxes[2] = createTextBox(D3DXVECTOR2(0.5*(0.4*SCREEN_WIDTH),0.5*(0.95*SCREEN_HEIGHT)), (wchar_t*)wStats[2].c_str(), 18, 0xffffffff);
	this->textBoxes[3] = createTextBox(D3DXVECTOR2(0.5*(0.4*SCREEN_WIDTH),0.5*(1.05*SCREEN_HEIGHT)), (wchar_t*)wStats[3].c_str(), 18, 0xffffffff);
	this->textBoxes[4] = createTextBox(D3DXVECTOR2(0.5*(0.4*SCREEN_WIDTH),0.5*(1.15*SCREEN_HEIGHT)), (wchar_t*)wStats[4].c_str(), 18, 0xffffffff);
	this->textBoxes[5] = createTextBox(D3DXVECTOR2(0.5*(0.4*SCREEN_WIDTH),0.5*(1.25*SCREEN_HEIGHT)), (wchar_t*)wStats[5].c_str(), 18, 0xffffffff);
	this->textBoxes[6] = createTextBox(D3DXVECTOR2(0.5*(0.4*SCREEN_WIDTH),0.5*(1.35*SCREEN_HEIGHT)), (wchar_t*)wStats[6].c_str(), 18, 0xffffffff);
	this->textBoxes[7] = createTextBox(D3DXVECTOR2(0.5*(0.4*SCREEN_WIDTH),0.5*(1.45*SCREEN_HEIGHT)), (wchar_t*)wStats[7].c_str(), 18, 0xffffffff);
	//this->textBoxes[8] = createTextBox(D3DXVECTOR2(0.5*(0.9*SCREEN_WIDTH),0.5*(1.05*SCREEN_HEIGHT)), (wchar_t*)wStats[8].c_str(), 18, 0xffffffff);
	//this->textBoxes[9] = createTextBox(D3DXVECTOR2(0.5*(0.9*SCREEN_WIDTH),0.5*(1.1*SCREEN_HEIGHT)), (wchar_t*)wStats[9].c_str(), 18, 0xffffffff);

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

void GUI::setInGameText(int resource, int supply, float currPercent, float winPercent, int selectedStructure)
{
	string name;
	string purpose;
	int cost;
	if(selectedStructure == BUILDABLE_MAINBUILDING)
	{
		name = "Headquarter";
		purpose = "Base";
		cost = 0;
		currentBuilding = TEXTURE_MAINBUILDING;
	}
	else if(selectedStructure == BUILDABLE_SUPPLY)
	{
		name = "Supply";
		purpose = "Give supply";
		cost = COST_SUPPLY;
		currentBuilding = TEXTURE_SUPPLY;
	}
	else if(selectedStructure == BUILDABLE_TOWER)
	{
		name = "Tower";
		purpose = "Offensive structure";
		cost = COST_TOWER;
		currentBuilding = TEXTURE_TOWER;
	}
	else if(selectedStructure == BUILDABLE_UPGRADE_OFFENSE)
	{
		name = "Offensive";
		purpose = "Upgrade";
		cost = COST_UPGRADE;
		currentBuilding = TEXTURE_ATK;
	}
	else if(selectedStructure == BUILDABLE_UPGRADE_DEFENSE)
	{
		name = "Defensive";
		purpose = "Upgrade";
		cost = COST_UPGRADE;
		currentBuilding = TEXTURE_DEF;
	}
	else if(selectedStructure == BUILDABLE_UPGRADE_RES)
	{
		name = "Resources";
		purpose = "Increase res/kill";
		cost = COST_UPGRADE;
		currentBuilding = TEXTURE_MONEY;
	}
		
	if(first)
	{
		this->nrOfBoxes = 8;
		this->textBoxes = new Text[this->nrOfBoxes];
		
	}
	char temp[255];
	string Stats[8];
	Stats[0] = itoa(resource, temp, 10);
	Stats[1] = itoa(supply, temp, 10);
	Stats[2] = itoa(this->endStats->totalTime, temp, 10);
	Stats[3] = "Built: ";
	Stats[3].append( convertFloat(currPercent) );
	Stats[4] = "Goal: ";
	Stats[4].append( convertFloat(winPercent) );
	Stats[5] = name;
	Stats[6] = purpose;
	Stats[7] = "Cost: ";
	Stats[7].append( itoa(cost, temp, 10) );
	

	for(int i = 0; i < 8; i++)
	{
		wStats[i] = convertStrToWstr(Stats[i]);
	}

	this->textBoxes[0] = createTextBox(D3DXVECTOR2(0.5*(1*SCREEN_WIDTH),0.5*(1.7*SCREEN_HEIGHT)), (wchar_t*)wStats[0].c_str(), 18, 0xffffffff);
	this->textBoxes[1] = createTextBox(D3DXVECTOR2(0.5*(1*SCREEN_WIDTH),0.5*(1.875*SCREEN_HEIGHT)), (wchar_t*)wStats[1].c_str(), 18, 0xffffffff);
	this->textBoxes[2] = createTextBox(D3DXVECTOR2(0.5*(1.3*SCREEN_WIDTH),0.5*(1.7*SCREEN_HEIGHT)), (wchar_t*)wStats[2].c_str(), 18, 0xffffffff);
	this->textBoxes[3] = createTextBox(D3DXVECTOR2(0.5*(1.65*SCREEN_WIDTH),0.5*(1.7*SCREEN_HEIGHT)), (wchar_t*)wStats[3].c_str(), 18, 0xffffffff);
	this->textBoxes[4] = createTextBox(D3DXVECTOR2(0.5*(1.85*SCREEN_WIDTH),0.5*(1.7*SCREEN_HEIGHT)), (wchar_t*)wStats[4].c_str(), 18, 0xffffffff);
	this->textBoxes[5] = createTextBox(D3DXVECTOR2(0.5*(0.5*SCREEN_WIDTH),0.5*(1.7*SCREEN_HEIGHT)), (wchar_t*)wStats[5].c_str(), 18, 0xffffffff);
	this->textBoxes[6] = createTextBox(D3DXVECTOR2(0.5*(0.51*SCREEN_WIDTH),0.5*(1.8*SCREEN_HEIGHT)), (wchar_t*)wStats[6].c_str(), 14, 0xffffffff);
	this->textBoxes[7] = createTextBox(D3DXVECTOR2(0.5*(0.5*SCREEN_WIDTH),0.5*(1.9*SCREEN_HEIGHT)), (wchar_t*)wStats[7].c_str(), 14, 0xffffffff);

	this->panels[0].textureID = currentBuilding;
}

void GUI::restartStats()
{
	endStats->shutdown();
	endStats = endStats->Getinstance();
}