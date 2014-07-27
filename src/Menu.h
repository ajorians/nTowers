#ifndef MENU_H
#define MENU_H

#define SHOW_LEVEL_DIMENSIONS
//#define SHOW_PREVIEW

extern "C"
{
	#include <os.h>
	#include "SDL/SDL.h"
}

#include "Config.h"

class MainMenu
{
public:
	MainMenu(SDL_Surface* pScreen, Config* pConfig);
	~MainMenu();

	bool Loop();

	bool ShouldQuit() const;
	bool ShowShowOptions() const;
	bool ShouldShowHelp() const;
	const char* GetLevel();
	int GetLevelNumber() const;
	
protected:
	bool PollEvents();
	void UpdateDisplay();
	void UpdateLevelDimensions();
#ifdef SHOW_PREVIEW
	void FreePreviewSurface();
#endif

protected:
	SDL_Surface	*m_pScreen;//Does not own
	SDL_Surface 	*m_pBackground;
	nSDL_Font 	*m_pFont;
	enum MenuChoice{Play, Options, Help, Quit};
	MenuChoice 	m_eChoice;
	Config		*m_pConfig;
	static int 	s_nLevel;
	bool		m_bBeatLevel;
	int		m_nCompletionTime;
	SDL_Surface	*m_pStar;
#ifdef SHOW_LEVEL_DIMENSIONS
	int 		m_nWidth;
	int 		m_nHeight;
#endif
#ifdef SHOW_PREVIEW
	SDL_Surface     *m_pPreview;
#endif
};

#endif
