#ifndef GAME_H
#define GAME_H

//#define USE_GRAPHIC_YOU_WIN//Use a static "You Win!!!" image instead of the animated one

extern "C"
{
	#include <os.h>
	#include "SDL/SDL.h"
	#include "TowersLib/TowersLib.h"
}

#ifndef USE_GRAPHIC_YOU_WIN
#include "Message.h"
#endif

#include "Background.h"
#include "Config.h"
#include "Selector.h"
#include "Metrics.h"
#include "Timer.h"
#include "GameDrawer.h"
#include "Defines.h"

class Game
{
public:
	Game(SDL_Surface* pScreen, TowersLib towers, int nLevelNumber, Config* pConfig);
	~Game();

	bool Loop();
	
protected:
	bool CheckStateConditions();
	bool PollEvents();
	void DrawSelector();
	void UpdateDisplay();
	void Move(Direction eDirection);
	void SetSpot(int nValue);
	void IncrementSpot();
	void Undo();
	void Redo();

protected:
	SDL_Surface	*m_pScreen;//Does not own
	Background	m_Background;
#ifdef USE_GRAPHIC_YOU_WIN
	SDL_Surface 	*m_pWinGraphic;
#else
	Message    m_YouWinMessage;
#endif
	GameDrawer	m_Drawer;
	TowersLib	m_Towers;
	int		m_nLevelNumber;
	Config		*m_pConfig;//Does not own
	Selector	m_Selector;
	bool		m_bGameOver;
	Metrics		m_BoardMetrics;
	Timer		m_Timer;
};

#endif
