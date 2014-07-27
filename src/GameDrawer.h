#ifndef GAMEDRAWER_H
#define GAMEDRAWER_H

extern "C"
{
        #include <os.h>
        #include "SDL/SDL.h"
        #include "TowersLib/TowersLib.h"
}

class Metrics;

class GameDrawer
{
public:
   GameDrawer(SDL_Surface* pSurface, TowersLib tower, Metrics& metrics);
   ~GameDrawer();
   void Draw();

protected:
   void DrawCell(int nX, int nY);
   void DrawIndicator(int nIndex);

protected:
   SDL_Surface	*m_pSurface;
   TowersLib	m_Towers;
   Metrics	*m_pBoardMetrics;

   nSDL_Font    *m_pFont;
   nSDL_Font    *m_pFontBad;
   nSDL_Font    *m_pFontGood;
};

#endif

